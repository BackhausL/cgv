#include "vr_view_interactor.h"
#include <cgv/render/attribute_array_binding.h>
#include <cgv/render/shader_program.h>
#include <cgv/gui/trigger.h>
#include <cgv/signal/rebind.h>
#include <cgv/math/ftransform.h>
#include <cgv/base/import.h>
#include <cgv/math/inv.h>
#include <iostream>
#include <sstream>
#include <vr/vr_kit.h>
#include <vr/vr_driver.h>

#include <plugins/openvr_driver/openvr_kit.h>
#include <cgv_gl/rectangle_renderer.h>

///
vr_view_interactor::vr_view_interactor(const char* name) : stereo_view_interactor(name),
fence_color1(0, 0, 1), fence_color2(1, 1, 0)
{
	debug_vr_events = false;
	separate_view = true;
	blit_vr_views = true;
	blit_width = 160;
	event_flags = cgv::gui::VREventTypeFlags(cgv::gui::VRE_STATUS + cgv::gui::VRE_KEY + cgv::gui::VRE_POSE);
	rendered_kit_ptr = 0;
	rendered_eye = 0;
	rendered_kit_index = -1;

	fence_frequency = 5;
	fence_line_width = 3;
	show_vr_kits = true;
	show_vr_kits_as_spheres = false;
	show_vr_kits_as_meshes = true;

	show_action_zone = false;
	current_vr_handle = 0;
	current_vr_handle_index = 0;
	kit_enum_definition = "enums='none=0'";

	brs.map_color_to_material = cgv::render::MS_FRONT_AND_BACK;
	srs.map_color_to_material = cgv::render::MS_FRONT_AND_BACK;
	srs.blend_width_in_pixel = 0;

	left_eye_camera_texture = texture(
	"uint8[R,G,B,A]", TF_LINEAR, TF_LINEAR, TW_CLAMP_TO_EDGE, TW_CLAMP_TO_EDGE);
	right_eye_camera_texture = texture(
		"uint8[R,G,B,A]", TF_LINEAR, TF_LINEAR, TW_CLAMP_TO_EDGE, TW_CLAMP_TO_EDGE);
	camera_texture_zoom_x = 0.0f;
	camera_texture_zoom_y = 0.0f;
	camera_texture_offset_x = 0.0f;
	camera_texture_offset_y = 0.0f;
	camera_left_eye_offset_x = 0.0f;
	camera_left_eye_offset_y = 0.0f;
	camera_right_eye_offset_x = 0.0f;
	camera_right_eye_offset_y = 0.0f;

	cgv::signal::connect(cgv::gui::ref_vr_server().on_device_change, this, &vr_view_interactor::on_device_change);
	cgv::signal::connect(cgv::gui::ref_vr_server().on_status_change, this, &vr_view_interactor::on_status_change);
}

/// set whether vr events should be printed to the console window
void vr_view_interactor::enable_vr_event_debugging(bool enable)
{
	if (debug_vr_events != enable) {
		debug_vr_events = enable;
		on_set(&debug_vr_events);
	}
}

/// set whether to draw separate view
void vr_view_interactor::draw_separate_view(bool do_draw)
{
	if (separate_view != do_draw) {
		separate_view = do_draw;
		on_set(&separate_view);
	}
}

/// whether to draw vr kits
void vr_view_interactor::draw_vr_kits(bool do_draw)
{
	if (show_vr_kits != do_draw) {
		show_vr_kits = do_draw;
		on_set(&show_vr_kits);
	}
}
/// whether to draw action zone
void vr_view_interactor::draw_action_zone(bool do_draw)
{
	if (show_action_zone != do_draw) {
		show_action_zone = do_draw;
		on_set(&show_action_zone);
	}
}
/// enable vr view blitting
void vr_view_interactor::enable_blit_vr_views(bool enable)
{
	if (blit_vr_views != enable) {
		blit_vr_views = enable;
		on_set(&blit_vr_views);
	}
}
/// set the width with which vr views are blit
void vr_view_interactor::set_blit_vr_view_width(int width)
{
	if (blit_width != width) {
		blit_width = width;
		on_set(&blit_width);
	}
}

void vr_view_interactor::toogle_camera_seethrough()
{
	if(!current_vr_handle) return;

	vr::vr_kit* kit_ptr = vr::get_vr_kit(current_vr_handle);
	auto cam = kit_ptr->get_camera();

	if (cam) {
		auto s = cam->get_state();
		if (s == vr::camera_state::INITIALIZED /*|| s == vr::camera_state::ERROR*/) {
			auto b = cam->start();
		}
		else if (s == vr::camera_state::STARTED) {
			auto b = cam->stop();
		}
	}
}

/// return a pointer to the state of the current vr kit
const vr::vr_kit_state* vr_view_interactor::get_current_vr_state() const
{
	if (current_vr_handle_index > 0 && current_vr_handle_index - 1 < int(kit_states.size()))
		return &kit_states[current_vr_handle_index - 1];
	return 0;
}
/// query the currently set event type flags
cgv::gui::VREventTypeFlags vr_view_interactor::get_event_type_flags() const
{
	return event_flags;
}
/// set the event type flags of to be emitted events
void vr_view_interactor::set_event_type_flags(cgv::gui::VREventTypeFlags flags)
{
	event_flags = flags;
	on_set(&event_flags);
}

///
void vr_view_interactor::on_status_change(void* device_handle, int controller_index, vr::VRStatus old_status, vr::VRStatus new_status)
{
	post_redraw();
	if (debug_vr_events)
		std::cout << "on status change(" << device_handle << ","
		<< controller_index << "," << vr::get_status_string(old_status)
		<< "," << vr::get_status_string(new_status) << ")" << std::endl;
}

///
void vr_view_interactor::on_device_change(void* device_handle, bool attach)
{
	if (attach)
		new_kits.push_back(device_handle);
	else
		old_kits.push_back(device_handle);
	post_redraw();
	if (debug_vr_events)
		std::cout << "on device change(" << device_handle << ","
		<< (attach ? "attach" : "detach") << ")" << std::endl;
}

/// return the type name 
std::string vr_view_interactor::get_type_name() const
{
	return "vr_view_interactor";
}


void vr_view_interactor::camera_texture_zoom_changed()
{
	rect.set_zoom({ camera_texture_zoom_x, camera_texture_zoom_y });
}

void vr_view_interactor::camera_texture_offset_changed()
{
	rect.set_offset({ camera_texture_offset_x, camera_texture_offset_y });
}

/// 
void vr_view_interactor::on_set(void* member_ptr)
{
	if (member_ptr == &current_vr_handle_index) {
		if (current_vr_handle_index == 0) {
			current_vr_handle = 0;
			if (!separate_view) {
				separate_view = true;
				update_member(&separate_view);
			}
		}
		else
			if (current_vr_handle_index - 1 < int(kits.size()))
				current_vr_handle = kits[current_vr_handle_index - 1];
	}
	stereo_view_interactor::on_set(member_ptr);
}

/// overload to stream help information to the given output stream
void vr_view_interactor::stream_help(std::ostream& os)
{
	os << "vr_view_interactor: Ctrl-0|1|2|3 to select player; Ctrl-Space to toggle draw separate view\n";
	stereo_view_interactor::stream_help(os);
}

/// overload to show the content of this object
void vr_view_interactor::stream_stats(std::ostream& os)
{
	stereo_view_interactor::stream_stats(os);
}

bool vr_view_interactor::init(cgv::render::context& ctx)
{
	cgv::render::ref_sphere_renderer(ctx, 1);
	cgv::media::mesh::simple_mesh<float> M;
	if (M.read(cgv::base::find_data_file("vr_controller_vive_1_5.obj", "D")))
		MI_controller.construct(ctx, M);
	cgv::media::mesh::simple_mesh<float> M2;
	if (M2.read(cgv::base::find_data_file("generic_hmd.obj", "D")))
		MI_hmd.construct(ctx, M2);

	if (!MI_hmd.is_constructed() && !MI_controller.is_constructed()) {
		show_vr_kits_as_meshes = false;
		on_set(&show_vr_kits_as_meshes);
		if (!show_vr_kits_as_spheres) {
			show_vr_kits_as_spheres = true;
			on_set(&show_vr_kits_as_spheres);
		}
	}

	rect.init(ctx);

	return stereo_view_interactor::init(ctx);
}

void vr_view_interactor::destruct(cgv::render::context& ctx)
{
	cgv::render::ref_sphere_renderer(ctx, -1);
}

/// overload and implement this method to handle events
bool vr_view_interactor::handle(cgv::gui::event& e)
{
	if (debug_vr_events) {
		if ((e.get_flags() & cgv::gui::EF_VR) != 0) {
			e.stream_out(std::cout);
			std::cout << std::endl;
		}
	}
	if (e.get_kind() == cgv::gui::EID_KEY) {
		cgv::gui::key_event& ke = static_cast<cgv::gui::key_event&>(e);
		if ((ke.get_action() == cgv::gui::KA_PRESS) &&
			(ke.get_modifiers() == cgv::gui::EM_CTRL)) {
			if (ke.get_key() >= '0' && ke.get_key() < '4') {
				unsigned player_index = ke.get_key() - '0';
				if (player_index < kits.size()) {
					current_vr_handle_index = player_index + 1;
					current_vr_handle = kits[player_index];
					update_member(&current_vr_handle_index);
					return true;
				}
			}
			if (ke.get_key() == cgv::gui::KEY_Space) {
				separate_view = !separate_view;
				on_set(&separate_view);
				return true;
			}
		}
	}
	return stereo_view_interactor::handle(e);
}


/// this method is called in one pass over all drawables after drawing
void vr_view_interactor::finish_frame(cgv::render::context& ctx)
{
	stereo_view_interactor::finish_frame(ctx);
}

void vr_view_interactor::after_finish(cgv::render::context& ctx)
{
	stereo_view_interactor::after_finish(ctx);
	if (ctx.get_render_pass() == cgv::render::RP_MAIN) {
		if (rendered_kit_ptr) {
			rendered_kit_ptr->disable_fbo(rendered_eye);
			int width = ctx.get_width() / 2;
			int x0 = 0;
			int blit_height = width * rendered_kit_ptr->get_height() / rendered_kit_ptr->get_width();
			for (int eye = 0; eye < 2; ++eye) {
				rendered_kit_ptr->blit_fbo(eye, x0, 0, width, ctx.get_height());
				x0 += width;
			}
			rendered_eye = 0;
			rendered_kit_ptr = 0;
			rendered_kit_index = -1;
		}
		// blit vr kit views in main framebuffer
		if (kits.size() > unsigned(separate_view ? 0 : 1) && blit_vr_views) {
			int y0 = 0;
			for (auto handle : kits) {
				if (!separate_view && handle == current_vr_handle)
					continue;
				vr::vr_kit* kit_ptr = vr::get_vr_kit(handle);
				if (!kit_ptr)
					continue;
				int x0 = 0;
				int blit_height = blit_width * kit_ptr->get_height() / kit_ptr->get_width();
				for (int eye = 0; eye < 2; ++eye) {
					kit_ptr->blit_fbo(eye, x0, y0, blit_width, blit_height);
					x0 += blit_width + 5;
				}
				y0 += blit_height + 5;
				kit_ptr->submit_frame();
			}
		}
		if (current_vr_handle)
			post_redraw();
	}
}

void vr_view_interactor::configure_kits()
{
	bool update_kits = false;
	void* last_current_vr_handle = current_vr_handle;
	// check for old kits, destruct their render objects, unregister them and delete them
	while (!old_kits.empty()) {
		if (current_vr_handle == old_kits.back())
			current_vr_handle = 0;
		vr::vr_kit* kit_ptr = vr::get_vr_kit(old_kits.back());
		if (kit_ptr) {
			if (kit_ptr->fbos_initialized())
				kit_ptr->destruct_fbos();
			if (!vr::unregister_vr_kit(old_kits.back(), kit_ptr)) {
				std::cerr << "unable to unregister removed kit <" << old_kits.back() << ">" << std::endl;
			}
			delete kit_ptr;
		}
		auto iter = std::find(kits.begin(), kits.end(), old_kits.back());
		if (iter != kits.end()) {
			kits.erase(iter);
			update_kits = true;
		}
		old_kits.pop_back();
	}
	if (current_vr_handle == 0 && !kits.empty())
		current_vr_handle = kits.front();
	// check for new kits and initialize their render objects in current context
	while (!new_kits.empty()) {
		vr::vr_kit* kit_ptr = vr::get_vr_kit(new_kits.back());
		if (kit_ptr) {
			if (!kit_ptr->fbos_initialized())
				if (kit_ptr->init_fbos()) {
					std::cout << "initialized fbos of " << kit_ptr->get_name() << std::endl;
					if (current_vr_handle == 0)
						current_vr_handle = new_kits.back();
				}
			kits.push_back(new_kits.back());
			update_kits = true;
		}
		new_kits.pop_back();
	}
	kit_states.resize(kits.size());
	// 
	if (update_kits) {
		kit_enum_definition = "enums='none=0";
		for (auto handle : kits) {
			vr::vr_kit* kit_ptr = vr::get_vr_kit(handle);
			std::string kit_name;
			if (kit_ptr)
				kit_name = kit_ptr->get_name();
			else {
				std::stringstream ss;
				ss << handle;
				kit_name = ss.str();
			}
			kit_enum_definition += ";";
			kit_enum_definition += kit_name;
		}
		kit_enum_definition += "'";
		if (find_control(current_vr_handle_index))
			find_control(current_vr_handle_index)->multi_set(kit_enum_definition);
		std::cout << kit_enum_definition << std::endl;
	}
	if (update_kits || current_vr_handle != last_current_vr_handle) {
		if (current_vr_handle == 0)
			current_vr_handle_index = 0;
		else {
			for (unsigned i = 0; i < kits.size(); ++i)
				if (kits[i] == current_vr_handle) {
					current_vr_handle_index = i + 1;
					break;
				}
		}
		update_member(&current_vr_handle_index);
	}
}

vr_view_interactor::dmat4 vr_view_interactor::hmat_from_pose(float pose_matrix[12])
{
	dmat4 M;
	M.set_col(0, dvec4(reinterpret_cast<vec3&>(pose_matrix[0]), 0));
	M.set_col(1, dvec4(reinterpret_cast<vec3&>(pose_matrix[3]), 0));
	M.set_col(2, dvec4(reinterpret_cast<vec3&>(pose_matrix[6]), 0));
	M.set_col(3, dvec4(reinterpret_cast<vec3&>(pose_matrix[9]), 1));
	return M;
}

/// this method is called in one pass over all drawables before the draw method
void vr_view_interactor::init_frame(cgv::render::context& ctx)
{

	cgv::render::RenderPassFlags rpf = ctx.get_render_pass_flags();
	if (ctx.get_render_pass() == cgv::render::RP_MAIN) {

		configure_kits();
		// perform rendering from the vr kits
		if (kits.size() > 0) {
			// query states
			vr::vr_kit* current_kit_ptr = 0;
			if (current_vr_handle_index > 0) {
				current_kit_ptr = vr::get_vr_kit(current_vr_handle);
				if (current_kit_ptr) {
					current_kit_ptr->query_state(kit_states[current_vr_handle_index - 1], 2);
					cgv::gui::ref_vr_server().check_new_state(current_vr_handle, kit_states[current_vr_handle_index - 1], cgv::gui::trigger::get_current_time(), event_flags);


				}
			}
			for (unsigned i = 0; i < kits.size(); ++i) {
				vr::vr_kit* kit_ptr = vr::get_vr_kit(kits[i]);
				if (!kit_ptr)
					continue;
				if (kit_ptr == current_kit_ptr)
					continue;
				kit_ptr->query_state(kit_states[i], 1);
				cgv::gui::ref_vr_server().check_new_state(kits[i], kit_states[i], cgv::gui::trigger::get_current_time(), event_flags);
			}
			// render all but current vr kit views
			for (rendered_kit_index = 0; rendered_kit_index<int(kits.size()); ++rendered_kit_index) {
				if (rendered_kit_index + 1 == current_vr_handle_index)
					continue;
				rendered_kit_ptr = vr::get_vr_kit(kits[rendered_kit_index]);
				if (!rendered_kit_ptr)
					continue;
				for (rendered_eye = 0; rendered_eye < 2; ++rendered_eye) {
					rendered_kit_ptr->enable_fbo(rendered_eye);
					ctx.render_pass(cgv::render::RP_USER_DEFINED, cgv::render::RenderPassFlags(rpf&~cgv::render::RPF_HANDLE_SCREEN_SHOT), this);
					rendered_kit_ptr->disable_fbo(rendered_eye);
				}
			}
			// render current vr kit 
			rendered_kit_index = current_vr_handle_index - 1;
			rendered_kit_ptr = vr::get_vr_kit(kits[rendered_kit_index]);
			if (rendered_kit_ptr) {
				if (rendered_kit_ptr->has_camera() && 
					rendered_kit_ptr->get_camera()->get_state() == vr::camera_state::STARTED) {
					auto cam = rendered_kit_ptr->get_camera();
					auto qv = cam->query();

					if (cam->is_new_frame_available()) {
						auto data = cam->get_frame();

						auto num_cams = cam->get_num_cameras();
						num_cams = 2; // TODO: implement 1 and remove

						if (num_cams == 1) {
							data_format df;

							switch (cam->get_frame_format()) {
							case vr::camera_frame_format::RGBA:
								df = data_format(cam->get_frame_width(), cam->get_frame_height(),
									cgv::type::info::TI_UINT8, CF_RGBA);
								break;
							default:
								df = data_format(cam->get_frame_width(), cam->get_frame_height(),
									cgv::type::info::TI_UINT8, CF_RGBA);
							}

							// seems to only work on ref
							// TODO: check this
							auto dv = data_view(&df, cam->get_frame_ref().data());

							// calls replace if already created - TODO: check this
							left_eye_camera_texture.create(ctx,dv);
						}
						else if (num_cams == 2) {
							data_format df1, df2;

							switch (cam->get_frame_format()) {
							case vr::camera_frame_format::RGBA: 
							{
								auto split = cam->get_frame_split();

								if (split == vr::camera_frame_split::UP_DOWN) {
									df1 = data_format(cam->get_frame_width(), cam->get_frame_height() / 2,
										cgv::type::info::TI_UINT8, CF_RGBA);
									df2 = df1;
								}
								else if (split == vr::camera_frame_split::LEFT_RIGHT) {
									// TODO: line-wise copy
									throw std::logic_error("vr camera left-right split not implemented");
								}

								// seems to only work on ref
								// TODO: check this
								auto dv = data_view(&df1, cam->get_frame_ref().data());
								// calls replace if already created - TODO: check this
								left_eye_camera_texture.create(ctx, dv);

								auto data_ptr = cam->get_frame_ref().data() +
									static_cast<int>(cam->get_frame_width() * (cam->get_frame_height() / 2) * 4);
								dv = data_view(&df2, data_ptr);
								right_eye_camera_texture.create(ctx, dv);

								break;
							}
							default: {
								throw std::logic_error("vr camera invalid frame format");
							}
							}
						}
					}
				}
				for (rendered_eye = 0; rendered_eye < 2; ++rendered_eye) {
					rendered_kit_ptr->enable_fbo(rendered_eye);

					if (rendered_eye == 1 && !separate_view)
						break;
					ctx.render_pass(cgv::render::RP_USER_DEFINED, cgv::render::RenderPassFlags(rpf&~cgv::render::RPF_HANDLE_SCREEN_SHOT));
					
					if (rendered_kit_ptr->has_camera() &&
						rendered_kit_ptr->get_camera()->get_state() == vr::camera_state::STARTED) {
						rect.set_flipped(true);
						rect.set_draw_mode(rectangle_renderer::draw_mode::NORMAL);

						vec2 old_offset = rect.get_offset();

						if (rendered_eye == 0) {
							rect.set_offset(old_offset + vec2(camera_left_eye_offset_x,
								camera_left_eye_offset_y));
							rect.draw_fullscreen(ctx, left_eye_camera_texture);
						}
						else if (rendered_eye == 1) {
							rect.set_offset(old_offset - vec2(camera_right_eye_offset_x,
								camera_right_eye_offset_y));
							rect.draw_fullscreen(ctx, right_eye_camera_texture);
						}
						rect.set_offset(old_offset);
						
					}
					
					rendered_kit_ptr->disable_fbo(rendered_eye);
				}
			}
			if (separate_view) {
				rendered_kit_ptr = 0;
				rendered_kit_index = -1;
			}
		}
	}
	if (rendered_kit_ptr) {
		float eye_to_head[12];
		rendered_kit_ptr->put_eye_to_head_matrix(rendered_eye, eye_to_head);
		ctx.set_modelview_matrix(inv(hmat_from_pose(kit_states[rendered_kit_index].hmd.pose)*hmat_from_pose(eye_to_head)));

		mat4 P;
		rendered_kit_ptr->put_projection_matrix(rendered_eye, float(z_near_derived), float(z_far_derived), &P(0, 0));
		compute_clipping_planes(z_near_derived, z_far_derived, clip_relative_to_extent);
		ctx.set_projection_matrix(P);
	}
	else {
		if (kits.empty() || separate_view)
			stereo_view_interactor::init_frame(ctx);
	}
}

/// 
void vr_view_interactor::draw(cgv::render::context& ctx)
{
	double time = cgv::gui::trigger::get_current_time();
	if (show_vr_kits) {
		std::vector<vec4> spheres;
		std::vector<rgb> sphere_colors;
		for (auto handle : kits) {
			vr::vr_kit* kit_ptr = vr::get_vr_kit(handle);
			if (!kit_ptr)
				continue;
			vr::vr_kit_state state;
			vr::vr_kit_state* state_ptr = &state;
			if (handle == current_vr_handle)
				state_ptr = &kit_states[current_vr_handle_index - 1];
			else if (!kit_ptr->query_state(state, 1))
				continue;

			if (show_vr_kits_as_meshes && MI_hmd.is_constructed()) {
				if (kit_ptr != rendered_kit_ptr) {
					ctx.push_modelview_matrix();
					ctx.mul_modelview_matrix(
						cgv::math::pose4<float>(reinterpret_cast<const mat34&>(state_ptr->hmd.pose[0]))*
						cgv::math::translate4<float>(0, 0.1f, -0.1f)
					);
					MI_hmd.render_mesh(ctx);
					ctx.pop_modelview_matrix();
				}
			}
			if (show_vr_kits_as_spheres || !MI_hmd.is_constructed()) {
				float left_eye_to_head[12];
				float right_eye_to_head[12];
				kit_ptr->put_eye_to_head_matrix(0, left_eye_to_head);
				kit_ptr->put_eye_to_head_matrix(1, right_eye_to_head);
				const mat3& R_w_h = reinterpret_cast<const mat3&>(state_ptr->hmd.pose[0]);
				const vec3& p_w_h = reinterpret_cast<const vec3&>(state_ptr->hmd.pose[9]);


				const mat3& R_h_l = reinterpret_cast<const mat3&>(left_eye_to_head[0]);
				const vec3& p_h_l = reinterpret_cast<const vec3&>(left_eye_to_head[9]);

				const mat3& R_h_r = reinterpret_cast<const mat3&>(right_eye_to_head[0]);
				const vec3& p_h_r = reinterpret_cast<const vec3&>(right_eye_to_head[9]);

				vec4 s_l(0, 0, 0, 0.012f);
				vec4 s_r = s_l;
				reinterpret_cast<vec3&>(s_l) = R_w_h * p_h_l + p_w_h;
				reinterpret_cast<vec3&>(s_r) = R_w_h * p_h_r + p_w_h;

				if (kit_ptr != rendered_kit_ptr || rendered_eye != 0) {
					spheres.push_back(s_l);
					sphere_colors.push_back(rgb(1, 0, 0));
				}
				if (kit_ptr != rendered_kit_ptr || rendered_eye != 1) {
					spheres.push_back(s_r);
					sphere_colors.push_back(rgb(0, 0, 1));
				}
			}
			for (unsigned i = 0; i < 2; ++i) {
				if (show_vr_kits_as_meshes && MI_controller.is_constructed()) {
					ctx.push_modelview_matrix();
					ctx.mul_modelview_matrix(cgv::math::pose4<float>(reinterpret_cast<const mat34&>(state_ptr->controller[i].pose[0])));
					MI_controller.render_mesh(ctx);
					ctx.pop_modelview_matrix();
				}
				if (show_vr_kits_as_spheres || !MI_controller.is_constructed()) {
					const mat3& R_ci = reinterpret_cast<const mat3&>(state_ptr->controller[i].pose[0]);
					const vec3& p_ci = reinterpret_cast<const vec3&>(state_ptr->controller[i].pose[9]);
					spheres.push_back(vec4(p_ci, 0.04f));
					spheres.push_back(vec4(p_ci + 0.05f*R_ci.col(0), 0.01f));
					spheres.push_back(vec4(p_ci - 0.05f*R_ci.col(0), 0.01f));
					spheres.push_back(vec4(p_ci + 0.05f*R_ci.col(1), 0.01f));
					spheres.push_back(vec4(p_ci - 0.05f*R_ci.col(1), 0.01f));
					spheres.push_back(vec4(p_ci + 0.05f*R_ci.col(2), 0.01f));
					spheres.push_back(vec4(p_ci - 0.05f*R_ci.col(2), 0.01f));
					sphere_colors.push_back(rgb(0.5f + (1 - i)*0.5f, 0.5f, 0.5f + 0.5f*i));
					sphere_colors.push_back(rgb(1, 0, 0));
					sphere_colors.push_back(rgb(1, 0.5f, 0.5f));
					sphere_colors.push_back(rgb(0, 1, 0));
					sphere_colors.push_back(rgb(0.5f, 1, 0.5f));
					sphere_colors.push_back(rgb(0, 0, 1));
					sphere_colors.push_back(rgb(0.5f, 0.5f, 1));
				}
			}
		}
		if (!spheres.empty()) {
			cgv::render::sphere_renderer& sr = cgv::render::ref_sphere_renderer(ctx);
			sr.set_y_view_angle(float(get_y_view_angle()));
			sr.set_render_style(srs);
			sr.set_sphere_array(ctx, spheres);
			sr.set_color_array(ctx, sphere_colors);
			sr.validate_and_enable(ctx);
			glDrawArrays(GL_POINTS, 0, GLsizei(spheres.size()));
			sr.disable(ctx);
		}
	}
	if (show_action_zone && current_vr_handle) {
		vr::vr_kit* kit_ptr = vr::get_vr_kit(current_vr_handle);
		if (kit_ptr) {
			const vr::vr_driver* driver_ptr = kit_ptr->get_driver();
			if (driver_ptr) {
				float h = driver_ptr->get_action_zone_height();
				vec3 up_dir;
				driver_ptr->put_up_direction(&up_dir[0]);
				std::vector<float> boundary;
				driver_ptr->put_action_zone_bounary(boundary);
				unsigned n = boundary.size() / 3;
				std::vector<vec3> G;
				G.resize(5 * n);
				unsigned i;
				for (i = 0; i < 5; ++i) {
					for (unsigned j = 0; j < n; ++j) {
						vec3 p = reinterpret_cast<const vec3&>(boundary[3 * j]);
						p += 0.25f*i*h*up_dir;
						G[i*n + j] = p;
					}
				}
				glLineStipple(3, GLushort(0xF0F0));
				glEnable(GL_LINE_STIPPLE);
				glLineWidth(fence_line_width);
				auto& prog = ctx.ref_default_shader_program();
				int pos_idx = prog.get_position_index();
				cgv::render::attribute_array_binding::set_global_attribute_array(ctx, pos_idx, G);
				cgv::render::attribute_array_binding::enable_global_array(ctx, pos_idx);
				prog.enable(ctx);
				float lambda = float(cos(fence_frequency*time));
				lambda *= lambda;
				ctx.set_color((1 - lambda)*fence_color1 + lambda * fence_color2);
				for (i = 0; i < 5; ++i)
					glDrawArrays(GL_LINE_LOOP, i*n, n);
				prog.disable(ctx);
				cgv::render::attribute_array_binding::disable_global_array(ctx, pos_idx);
				glDisable(GL_LINE_STIPPLE);
				glLineWidth(1.0f);
				post_redraw();
			}
		}
	}
	stereo_view_interactor::draw(ctx);
}

/// you must overload this for gui creation
void vr_view_interactor::create_gui()
{
	add_member_control(this, "current vr kit", (cgv::type::DummyEnum&)current_vr_handle_index, "dropdown", kit_enum_definition);
	if (begin_tree_node("VR rendering", separate_view, false, "level=2")) {
		align("\a");
		add_member_control(this, "separate_view", separate_view, "check");
		add_member_control(this, "blit_vr_views", blit_vr_views, "check");
		add_member_control(this, "blit_width", blit_width, "value_slider", "min=120;max=640;ticks=true;log=true");
		add_member_control(this, "show_action_zone", show_action_zone, "check");
		if (begin_tree_node("fence styles", fence_color1, false, "level=3")) {
			align("\a");
			add_member_control(this, "fence_color1", fence_color1);
			add_member_control(this, "fence_color2", fence_color2);
			add_member_control(this, "fence_line_width", fence_line_width, "value_slider", "min=1;max=20;ticks=true;log=true");
			add_member_control(this, "fence_frequency", fence_frequency, "value_slider", "min=0.1;max=10;ticks=true;log=true");
			align("\b");
			end_tree_node(fence_color1);
		}
		add_member_control(this, "show kits", show_vr_kits, "toggle", "w=62", " ");
		add_member_control(this, "as meshes", show_vr_kits_as_meshes, "toggle", "w=61", " ");
		add_member_control(this, "as spheres", show_vr_kits_as_spheres, "toggle", "w=61");
		if (begin_tree_node("sphere styles", srs, false, "level=3")) {
			align("\a");
			add_gui("sphere style", srs);
			align("\b");
			end_tree_node(srs);
		}
		align("\b");
		end_tree_node(separate_view);
	}
	if (begin_tree_node("VR events", event_flags, false, "level=2")) {
		align("\a");
		add_member_control(this, "debug_vr_events", debug_vr_events, "check");
		add_gui("event_flags", event_flags, "bit_field_control", "enums='dev=1,sta=2,key=4,thr=8,stk=16,skk=32,pos=64';gui_type='toggle';options='w=30';align=''");
		align("\n\b");
		end_tree_node(event_flags);
	}
	stereo_view_interactor::create_gui();

	if (begin_tree_node("Camera", show_camera_seethrough, true, "level=2")) {
		align("\a");
		connect_copy(add_button("Start | Stop")->click,
			cgv::signal::rebind(this, &vr_view_interactor::toogle_camera_seethrough));
		connect_copy(add_control("zoom x", camera_texture_zoom_x, "value_slider",
			"min=0.0;max=0.5;ticks=true")
			->value_change,
			rebind(this, &vr_view_interactor::camera_texture_zoom_changed));
		connect_copy(add_control("zoom y", camera_texture_zoom_y, "value_slider",
			"min=0.0;max=0.5;ticks=true")
			->value_change,
			rebind(this, &vr_view_interactor::camera_texture_zoom_changed));
		connect_copy(add_control("offset x", camera_texture_offset_x, "value_slider",
			"min=-0.5;max=0.5;ticks=true")
			->value_change,
			rebind(this, &vr_view_interactor::camera_texture_offset_changed));
		connect_copy(add_control("offset y", camera_texture_offset_y, "value_slider",
			"min=-0.5;max=0.5;ticks=true")
			->value_change,
			rebind(this, &vr_view_interactor::camera_texture_offset_changed));
		add_member_control(this, "left eye offset x", camera_left_eye_offset_x, "value_slider",
			"min=-0.5;max=0.5;ticks=true");
		add_member_control(this, "left eye offset y", camera_left_eye_offset_y, "value_slider",
			"min=-0.5;max=0.5;ticks=true");
		add_member_control(this, "right eye offset x", camera_right_eye_offset_x, "value_slider",
			"min=-0.5;max=0.5;ticks=true");
		add_member_control(this, "right eye offset y", camera_right_eye_offset_y, "value_slider",
			"min=-0.5;max=0.5;ticks=true");

		align("\n\b");
		end_tree_node(event_flags);
	}
}

/// you must overload this for gui creation
bool vr_view_interactor::self_reflect(cgv::reflect::reflection_handler& srh)
{
	return stereo_view_interactor::self_reflect(srh);
}


#include <cgv/base/register.h>

/// register a newly created cube with the name "cube1" as constructor argument
extern cgv::base::object_registration_1<vr_view_interactor, const char*>
obj1("vr interactor", "registration of vr interactor");

