#include "model.hpp"

#include <sstream>

model::model()
{
	// EDIT: replace this with what is needed by the application.
	node_group_descriptor group_a{
		"Arithmetic",
		{
			{
				"Add",
				{"Left", "Right"},
				{"Sum"}
			},
			{
				"Subtract",
				{"Left", "Right "},
				{"Difference"}
			}
		}
	};

	node_group_descriptors.emplace_back(std::move(group_a));
}

int model::add_node_instance(const node_descriptor* descriptor)
{
	const auto id = generate_id();

	std::vector<int> inputs(descriptor->inputs.size());

	std::vector<int> outputs(descriptor->outputs.size());

	for (size_t i = 0; i < descriptor->inputs.size(); i++)
		inputs[i] = generate_id();

	for (size_t i = 0; i < descriptor->outputs.size(); i++)
		outputs[i] = generate_id();

	node_instance instance{id, descriptor, std::move(inputs), std::move(outputs)};

	node_instances.emplace_back(std::move(instance));

	return id;
}

int model::add_link(int start, int end)
{
	const auto id = generate_id();

	links.emplace_back(link{id, start, end});

	return id;
}

void model::remove_link(const int link_id)
{
	for (auto it = links.begin(); it != links.end(); it++)
	{
		if (it->id == link_id)
		{
			links.erase(it);
			return;
		}
	}
}
