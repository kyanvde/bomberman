#include "TestFactory.h"
#include "TestRunner.h"
#include "World.h"

#include <cstdio>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

void runWorldLoaderExceptionTests(tests::TestRunner& runner) {
    using core::World;

    // --- A missing world file throws rather than crashing or silently producing an empty world. ---
    {
        const std::string path = "world_loader_exception_tests_missing.txt";
        std::remove(path.c_str());

        bool threw = false;
        try {
            World world(std::make_shared<tests::TestFactory>(), path);
        } catch (const std::runtime_error&) {
            threw = true;
        }
        runner.check(threw, "Loading a missing world file throws std::runtime_error");
    }

    // --- Inconsistent row lengths are rejected. ---
    {
        const std::string path = "world_loader_exception_tests_ragged.txt";
        {
            std::ofstream file(path);
            file << "WWW\n";
            file << "WW\n"; // shorter than the first row
            file << "WWW\n";
        }

        bool threw = false;
        try {
            World world(std::make_shared<tests::TestFactory>(), path);
        } catch (const std::runtime_error&) {
            threw = true;
        }
        std::remove(path.c_str());
        runner.check(threw, "Loading a world file with inconsistent row lengths throws std::runtime_error");
    }

    // --- An unrecognized cell character is rejected. ---
    {
        const std::string path = "world_loader_exception_tests_unknown_cell.txt";
        {
            std::ofstream file(path);
            file << "WWW\n";
            file << "W?W\n"; // '?' is not a recognized cell type
            file << "WWW\n";
        }

        bool threw = false;
        try {
            World world(std::make_shared<tests::TestFactory>(), path);
        } catch (const std::runtime_error&) {
            threw = true;
        }
        std::remove(path.c_str());
        runner.check(threw, "Loading a world file with an unrecognized cell character throws std::runtime_error");
    }

    // --- A null factory is rejected before any file I/O is attempted. ---
    {
        const std::string path = "world_loader_exception_tests_nullfactory.txt";
        {
            std::ofstream file(path);
            file << "WWW\n";
            file << "WWW\n";
            file << "WWW\n";
        }

        bool threw = false;
        try {
            World world(nullptr, path);
        } catch (const std::runtime_error&) {
            threw = true;
        }
        std::remove(path.c_str());
        runner.check(threw, "Constructing a World with a null factory throws std::runtime_error");
    }

    // --- An empty world file is rejected. ---
    {
        const std::string path = "world_loader_exception_tests_empty.txt";
        { std::ofstream file(path); } // zero bytes

        bool threw = false;
        try {
            World world(std::make_shared<tests::TestFactory>(), path);
        } catch (const std::runtime_error&) {
            threw = true;
        }
        std::remove(path.c_str());
        runner.check(threw, "Loading an empty world file throws std::runtime_error");
    }
}
