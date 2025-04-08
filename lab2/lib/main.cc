#include "lib/window.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map> 
#include <numeric>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>

// ========================
// ==== node structure ====
// ========================

namespace
{

    std::vector<Node_Ptr> roots { };

    std::unordered_map<int, Node_Ptr> nodes { };
    std::unordered_map<int, std::unordered_set<int>> incoming { };
    std::unordered_map<int, std::unordered_set<int>> outgoing { };

    void insert_root(Node_Ptr node)
    {
        auto it = std::find(roots.begin(), roots.end(), node);
        if (it == roots.end())
            roots.push_back(node);
    }
    
    void destroy_root(Node_Ptr node)
    {
        auto it = std::remove(roots.begin(), roots.end(), node);
        roots.erase(it, roots.end());
    }

    bool create_link(Node_Ptr from, Node_Ptr to)
    {
        if (from == to || from == nullptr || to == nullptr)
            return false;

        // insert link into global table
        incoming[  to->value].insert(from->value);
        outgoing[from->value].insert(  to->value);

        // TODO: Remove as root (if possible)

        from->insert(to);

        return true;
    }

    bool destroy_link(Node_Ptr from, Node_Ptr to)
    {
        // remove from global tables
        outgoing[from->value].erase(  to->value);
        incoming[  to->value].erase(from->value);

        from->remove(to);

        return true;
    }

    Node_Ptr create_node()
    {
        static int free_id { 0 };
        int ID { free_id++ };

        // allocate the node
        Node_Ptr node { new Node_Type { ID } };

        // insert into tables
        nodes[ID] = node;
        incoming[ID] = { };
        outgoing[ID] = { };

        // insert into the root structure
        roots.push_back(node);

        return node;
    }

    bool destroy_node(Node_Ptr node)
    {
        std::unordered_set<int> incoming_set = incoming[node->value];
        std::unordered_set<int> outgoing_set = outgoing[node->value];
        
        // remove all outgoing links
        for (int ID : outgoing_set)
            destroy_link(node, nodes[ID]);

        // remove all incoming links
        for (int ID : incoming_set)
            destroy_link(nodes[ID], node);
            
        // remove from table
        nodes.erase(node->value);
        destroy_root(node);

        return true;
    }

}

// ====================
// ==== user input ====
// ====================

namespace
{

    std::istringstream iss { };
    std::string cursor { ">" };

    // get the next user input
    template <typename T>
    bool next_input(T& input)
    {
        if (iss >> input)
            return true;

        // we've reached the end of the current line
        if (iss.eof())
        {
            // there is nothing more to read
            if (std::cin.eof()) return false;

            iss.clear(); // reset the flags

            // show the current cursor
            std::cout << cursor << " ";
            
            // read the next line into iss
            std::string line { };
            if (std::getline(std::cin, line))
            {
                iss.str(line);
                return next_input(input);
            }
        }

        // the read failed for some other reason
        return false;
    }

    // get the next user input, if failed: call (*error)
    template <typename T>
    bool next_input(T& input, void(*error)())
    {
        if (next_input(input))
            return true;

        (*error)();
        return false;
    }

    void expected_node_id()
    {
        std::cerr << "ERROR: Expected node ID, got something else..." << std::endl;

        iss.clear();
        iss.ignore(std::numeric_limits<std::streamsize>::max());
        iss.setstate(std::ios::eofbit);
    }

    Node_Ptr read_node()
    {
        int ID { };
        if (next_input(ID, &expected_node_id))
        {
            auto it = nodes.find(ID);
            if (it != nodes.end())
                return it->second;

            std::cerr << "ERROR: Unknown node '" << ID << "'" << std::endl;
        }

        iss.setstate(std::ios::eofbit);
        return nullptr;
    }

    std::pair<Node_Ptr, Node_Ptr> read_link()
    {
        std::string save { cursor };

        Node_Ptr from { };
        Node_Ptr   to { };

        cursor = "Enter ID of node where link starts:";
        from = read_node();

        if (from != nullptr)
        {
            cursor = "Enter ID of node where link ends:";
            to = read_node();

            if (to == nullptr)
                from = nullptr;
        }

        cursor = save;
        return { from, to };
    }

}

// ===================
// ==== interface ====
// ===================

namespace
{

    void help();
    void quit();
    void show();
    void goto_main_menu();

    // global mode
    void insert_mode();
    void remove_mode();

    // insert mode
    void insert_node();
    void insert_link();
    
    // remove mode
    void remove_node();
    void remove_link();

    enum struct Mode
    {
        Global,
        Insert,
        Remove
    };

    std::unordered_map<Mode, std::string> const mode_title = {
        { Mode::Global, "Main menu" },
        { Mode::Insert, "Insert mode" },
        { Mode::Remove, "Remove mode" }
    };

    std::unordered_map<Mode, std::string> const mode_description = {
        { Mode::Global, "Select what you wish to do with the directed graph." },
        { Mode::Insert, "Insert new nodes or links into the graph." },
        { Mode::Remove, "Remove nodes or links from the graph." }
    };

    struct Command_Entry
    {
        std::string description;
        void (*callback)();
    };

    using Command_Table = std::map<std::string, Command_Entry> const;
    Command_Table global_command_table = {
        { "help", { "Get help and list all available commands."     , &help } },
        { "quit", { "Quit the program."                             , &quit } },
        { "show", { "Show the window (if it isn't already visible).", &show } }
    };

    std::unordered_map<Mode, Command_Table> const command_table = {
        { Mode::Global, {
                { "insert", { "Go to insert mode.", &insert_mode } },
                { "remove", { "Go to remove mode.", &remove_mode } }
            } },
        { Mode::Insert, {
                { "node", { "Create a new node.", &insert_node } },
                { "link", { "Create a new link.", &insert_link } },
                { "done", { "Go back to mode selector.", &goto_main_menu } }
            } },
        { Mode::Remove, {
                { "node", { "Destroy an existing node.", &remove_node } },
                { "link", { "Destroy an existing link.", &remove_link } },
                { "done", { "Go back to mode selector.", &goto_main_menu } }
            } }
    };

    Mode current_mode { Mode::Global };

    bool execute_command(std::string const& command, Command_Table const& table)
    {
        auto it = table.lower_bound(command);
        if (it->first == command || it->first.substr(0, 1) == command)
        {
            std::lock_guard<std::mutex> const lock { redraw_mutex };
            (*it->second.callback)();
            return true;
        }
        return false;
    }

    bool execute_command(std::string const& command)
    {
        if (execute_command(command, global_command_table))
            return true;

        return execute_command(command, command_table.at(current_mode));
    }

    void run_interface()
    {
        std::cout << "Welcome to the graph drawer in the course TDDI82 at Linköping University!\n"
                  << "\n";
        help();

        std::string command { };
        while (next_input(command))
        {
            if (!execute_command(command))
                std::cerr << "ERROR: Unknown command '" << command << "'" << std::endl;
        }
    }

    void print_command_table(Command_Table const& table)
    {
        std::size_t width { 0 };
        for (auto&& [command, entry] : table)
            width = std::max(width, command.size());

        std::cout << std::left;
        for (auto&& [command, entry] : table)
        {
            std::cout << "    " << '[' << command.front() << ']'
                      << std::setw(width - 1) << command.substr(1) << " - "
                      << entry.description << std::endl;
        }
    }

    void help()
    {
        std::cout << "Mode: " << mode_title.at(current_mode) << std::endl
                  << "\n"
                  << mode_description.at(current_mode) << std::endl
                  << "Commands available in this mode:\n"
                  << "\n";
        print_command_table(command_table.at(current_mode));
        std::cout << "\n"
                  << "These are the globally available commands:\n"
                  << "\n";
        print_command_table(global_command_table);
        std::cout << "\n"
                  << "Where the brackets denotes short-hand names of the commands."
                  << std::endl;


    }

    void quit()
    {
        iss.setstate(std::ios::eofbit);
        std::cin.setstate(std::ios::eofbit);
    }

    void show()
    {
        should_redraw = true;
    }
    
    void goto_main_menu()
    {
        cursor = ">";
        current_mode = Mode::Global;
    }

    // global mode
    void insert_mode()
    {
        cursor = "insert >";
        current_mode = Mode::Insert;
    }

    void remove_mode()
    {
        cursor = "remove >";
        current_mode = Mode::Remove;
    }

    // insert mode
    void insert_node()
    {
        Node_Ptr node { create_node() };

        should_redraw = true;
        std::cout << "Successfully created node with ID " << node->value << std::endl;
    }

    void insert_link()
    {
        auto [from, to] = read_link();
        if (from != nullptr && to != nullptr)
        {
            if (create_link(from, to))
            {
                should_redraw = true;
                std::cout << "Successfully created link from node "
                          << from->value << " to node " << to->value << std::endl;
            }
        }
    }
    
    // remove mode
    void remove_node()
    {
        std::string save { cursor };
        cursor = "Enter ID of node to remove:";

        Node_Ptr node { read_node() };
        if (node != nullptr)
        {
            destroy_node(node);

            should_redraw = true;
            std::cout << "Successfully destroyed node with ID " << node->value << std::endl;
        }

        cursor = save;
    }
    
    void remove_link()
    {
        auto [from, to] = read_link();
        if (from != nullptr && to != nullptr)
        {   
            if (destroy_link(from, to))
            {
                should_redraw = true;
                std::cout << "Successfully destroyed link from node "
                          << from->value << " to node " << to->value << std::endl;
            }
        }
    }

}

int main(int argc, char** argv)
{
    // spawn the window
    std::thread t { &run_window, std::cref(roots) };

    {
        std::ostringstream oss { };
        std::copy(argv + 1, argv + argc, std::ostream_iterator<std::string>(oss, " "));
        iss.str(oss.str());
    }

    run_interface();
    
    // once we are here, the window should close
    notify_exit();
    t.join();
}
