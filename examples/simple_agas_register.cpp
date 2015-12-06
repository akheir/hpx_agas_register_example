//  Copyright (c) 2015 Alireza Kheirkhahan
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_init.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>

////////////////////////////////////////////////////////////////////

char const* const help = "commands: register [string] [number], query [string], help, quit";

int hpx_main()
{
	{
		// Print out the available commands.
    	std::cout << help << std::endl << "> ";

    	// Enter the interpreter loop.
		std::string line;
        while (std::getline(std::cin, line))
        {
            boost::algorithm::trim(line);

            std::vector<std::string> cmd;
            boost::algorithm::split(cmd, line,
                boost::algorithm::is_any_of(" \t\n"),
                boost::algorithm::token_compress_on);

            if (!cmd.empty() && !cmd[0].empty())
            {
                // try to interpret the entered command
                if (cmd[0] == "register") {
                    if (cmd.size() == 3) {
                    	// Create an id based on entered number.
                    	hpx::id_type id = hpx::id_type(
								boost::lexical_cast<boost::uint64_t>(cmd[2]),
								static_cast<hpx::naming::id_type::management_type>(1));
                    	// Register the string with the id.
                    	hpx::register_with_basename(
                   			cmd[1], id, 0);
                    }
                    else {
                        std::cout << "error: invalid command '"
                                  << line << "'" << std::endl
                                  << help << std::endl;
                    }
                }
                else if (cmd[0] == "query") {
                    if (cmd.size() == 2) {
                    	hpx::id_type id = hpx::find_from_basename(cmd[1], 0).get();
                    	std::cout << cmd[1] << '\t' << id.get_lsb() << std::endl;
                    }
                    else {
                        std::cout << "error: invalid command '"
                                  << line << "'" << std::endl
                                  << help << std::endl;
                    }
                }
                else if (cmd[0] == "help") {
                    std::cout << help << std::endl;
                }
                else if (cmd[0] == "quit") {
                    break;
                }
                else {
                    std::cout << "error: invalid command '"
                              << line << "'" << std::endl
                              << help << std::endl;
                }
            }

            std:: cout << "> ";
        }
    }

    // Initiate shutdown of the runtime systems on all localities.
    return hpx::finalize();
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    // We force this example to use 2 threads by default as one of the threads
    // will be sitting most of the time in the kernel waiting for user input.
    std::vector<std::string> cfg;
    cfg.push_back("hpx.os_threads=2");

    // Initialize and run HPX.
    return hpx::init(argc, argv, cfg);
}
