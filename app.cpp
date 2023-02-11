#include "app.hpp"

#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <imnodes.h>

#include <limits>

#include "model.hpp"

class app final
{
public:
	explicit app(GLFWwindow* window)
		: window_(window)
	{
		glfwSwapInterval(1);

		glClearColor(0, 0, 0, 1);

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGui_ImplGlfw_InitForOpenGL(window_, true);

		ImGui_ImplOpenGL3_Init("#version 100");

		nodes_context_ = ImNodes::CreateContext();

		setup_style();
	}

	~app()
	{
		ImNodes::DestroyContext(nodes_context_);

		ImGui_ImplOpenGL3_Shutdown();

		ImGui_ImplGlfw_Shutdown();

		ImGui::DestroyContext();
	}

	void loop()
	{
		ImGui_ImplOpenGL3_NewFrame();

		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		int display_w, display_h;
		glfwGetFramebufferSize(window_, &display_w, &display_h);

		const auto menu_size = render_main_menu_bar();

		ImGui::SetNextWindowSize(ImVec2(display_w, display_h - menu_size.y), ImGuiCond_Always);

		ImGui::SetNextWindowPos(ImVec2(0, menu_size.y));

		ImGui::Begin("MainWindow", nullptr, ImGuiWindowFlags_NoDecoration);

		ImNodes::BeginNodeEditor();

		render_nodes();

		render_links();

		const auto editor_hovered = ImNodes::IsEditorHovered();

		ImNodes::EndNodeEditor();

		int pin_start{-1};
		int pin_end{-1};
		if (ImNodes::IsLinkCreated(&pin_start, &pin_end))
		{
			snapshot_model();
			get_current_model()->add_link(pin_start, pin_end);
		}

		int destroyed_link_id{-1};
		if (ImNodes::IsLinkDestroyed(&destroyed_link_id))
		{
			snapshot_model();
			get_current_model()->remove_link(destroyed_link_id);
		}

		const auto open_popup = editor_hovered && ImGui::IsKeyDown(ImGuiKey_A);

		// todo: delete popup?

		if (open_popup)
			ImGui::OpenPopup("Node Context Menu");

		if (ImGui::BeginPopup("Node Context Menu"))
		{
			render_add_node_menu();

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Render();

		glViewport(0, 0, display_w, display_h);

		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

private:
	void render_nodes() const
	{
		auto* m = get_current_model();

		for (const auto& n : m->node_instances)
		{
			ImNodes::BeginNode(n.id);

			ImNodes::BeginNodeTitleBar();

			ImGui::TextUnformatted(n.descriptor->name.c_str());

			ImNodes::EndNodeTitleBar();

			for (std::size_t i = 0; i < n.descriptor->inputs.size(); i++)
			{
				ImNodes::BeginInputAttribute(n.inputs[i]);

				ImGui::TextUnformatted(n.descriptor->inputs[i].c_str());

				ImNodes::EndInputAttribute();
			}

			for (std::size_t i = 0; i < n.descriptor->outputs.size(); i++)
			{
				ImNodes::BeginOutputAttribute(n.outputs[i]);

				ImGui::TextUnformatted(n.descriptor->outputs[i].c_str());

				ImNodes::EndOutputAttribute();
			}

			ImNodes::EndNode();
		}
	}

	void render_links() const
	{
		auto* m = get_current_model();

		for (const auto& l : m->links)
		{
			ImNodes::Link(l.id, l.start, l.end);
		}
	}

	void render_add_node_menu()
	{
		auto* m = get_current_model();

		const auto& p = ImGui::GetMousePosOnOpeningCurrentPopup();

		for (const auto& g : m->node_group_descriptors)
		{
			if (!ImGui::BeginMenu(g.name.c_str()))
				continue;

			for (const auto& n : g.nodes)
			{
				if (ImGui::Selectable(n.name.c_str()))
				{
					snapshot_model();

					m = get_current_model();

					const auto id = m->add_node_instance(&n);

					ImNodes::SetNodeScreenSpacePos(id, p);
				}
			}

			ImGui::EndMenu();
		}
	}

	void setup_style()
	{
		auto& s = ImGui::GetStyle();

		s.WindowBorderSize = 0.0f;
		s.PopupBorderSize = 0.0f;
		s.PopupRounding = 4.0f;
	}

	ImVec2 render_main_menu_bar()
	{
		ImVec2 window_size{0, 0};

		if (ImGui::BeginMainMenuBar())
		{
			window_size = ImGui::GetWindowSize();

			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
				}

				if (ImGui::MenuItem("Save"))
				{
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
				{
					glfwSetWindowShouldClose(window_, GLFW_TRUE);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo"))
				{
					undo();
				}

				if (ImGui::MenuItem("Redo"))
				{
					redo();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		return window_size;
	}

	model* get_current_model()
	{
		return &model_history_[current_model_];
	}

	const model* get_current_model() const
	{
		return &model_history_[current_model_];
	}

	void snapshot_model()
	{
		model_history_.resize(current_model_ + 1);

		model_history_.emplace_back(*get_current_model());
	}

	void undo()
	{
	}

	void redo()
	{
	}

	GLFWwindow* window_{nullptr};

	ImNodesContext* nodes_context_{nullptr};

	std::vector<model> model_history_{model{}};

	std::size_t current_model_{0u};
};

app* app_setup(GLFWwindow* window)
{
	return new app(window);
}

void app_teardown(app* self)
{
	delete self;
}

void app_loop(app* self)
{
	self->loop();
}
