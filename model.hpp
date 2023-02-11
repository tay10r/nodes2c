#pragma once

#include <string>
#include <vector>

/**
 * \brief Represents the data model of the flow.
 */
struct model final
{
	/**
	 * \brief Describes a single node.
	 */
	struct node_descriptor final
	{
		/**
		 * \brief The name of this node.
		 */
		std::string name;

		/**
		 * \brief The names of the input pins.
		 */
		std::vector<std::string> inputs;

		/**
		 * \brief The names of the output pins.
		 */
		std::vector<std::string> outputs;
	};

	/**
	 * \brief Used for describing a group of nodes.
	 */
	struct node_group_descriptor final
	{
		/**
		 * \brief The name of the node group.
		 */
		std::string name;

		/**
		 * \brief The nodes belonging to this group.
		 */
		std::vector<node_descriptor> nodes;
	};

	/**
	 * \brief An instance of a node.
	 */
	struct node_instance final
	{
		/**
		 * \brief The unique ID assigned to the node.
		 */
		int id{-1};

		/**
		 * \brief The descriptor that the node was made from.
		 */
		const node_descriptor* descriptor{nullptr};

		/**
		 * \brief The IDs assigned to each of the input pins.
		 */
		std::vector<int> inputs;

		/**
		 * \brief The IDs assigned to each of the output pins.
		 */
		std::vector<int> outputs;
	};

	/**
	 * \brief Represents a link between the input of one node and the output of another.
	 */
	struct link final
	{
		/**
		 * \brief The ID of the link itself.
		 */
		int id{-1};

		/**
		 * \brief The pin ID of the input node.
		 */
		int start{-1};

		/**
		 * \brief The pin ID of the output node.
		 */
		int end{-1};
	};

	/**
	 * \brief Constructs the list of known descriptors.
	 */
	model();

	/**
	 * \brief Creates a new node instance.
	 * \param descriptor The descriptor of the node being created.
	 * \return The ID of the newly generated node.
	 */
	int add_node_instance(const node_descriptor* descriptor);

	/**
	 * \brief Creates a new link.
	 * \param start The pin ID of the input node.
	 * \param end The pin ID of the output node.
	 * \return The ID of the newly created link.
	 */
	int add_link(int start, int end);

	void remove_link(int link_id);

	/**
	 * \brief The list of known node group descriptors.
	 */
	std::vector<node_group_descriptor> node_group_descriptors;

	/**
	 * \brief The list of instantiated nodes.
	 */
	std::vector<node_instance> node_instances;

	/**
	 * \brief The links between node instances.
	 */
	std::vector<link> links;

	int generate_id() { return next_id++; }

	int next_id{0};
};
