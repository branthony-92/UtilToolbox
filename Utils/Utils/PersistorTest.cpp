// PersistorTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdafx.h>
#include <PersistorTest.h>

#include <iostream>
#include <vector>
#include <array>
#include "Persistor.h"
#include "DirectoryTree.h"



int foo()
{
    try
    {
        CDirectoryTree directoryTree(WorkingDirectory::Config, "TestDirectory");
        auto pNode = directoryTree.getDir("JsonTest");

        if (!pNode)
        {
            throw std::runtime_error("Folder could not found/created");
        }

        auto path = pNode->getPathStr();

        CPersistor p("Test");
        p.setPath(path);

        CPersistor::ReturnStatus ret;
        ret = p.load();

        auto& j = p.getPersistenceObject();

        JSON& j1 = j["intVal"];

        auto has = j1.contains("hi");

        p.put({ "intVal"   }, 1);
        p.put({ "boolVal"  }, true);
        p.put({ "strVal"   }, "string");
        p.put({ "arrayVal" }, std::vector<int>{1, 2, 3, 4});

        p.put({ "subObject", "intVal"   }, 1);
        p.put({ "subObject", "boolVal"  }, true);
        p.put({ "subObject", "strVal"   }, "string");
        p.put({ "subObject", "arrayVal" }, std::vector<int>{1, 2, 3, 4});
        p.put({ "subObjectL1", "subObjectL2", "intVal" }, 1);
        p.put({ "subObjectL1", "subObjectL2", "boolVal" }, true);
        p.put({ "subObjectL1", "subObjectL2", "strVal" }, "string");
        p.put({ "subObjectL1", "subObjectL2", "arrayVal" }, std::vector<int>{1, 2, 3, 4});
        p.put({ "subObjectL1", "subObjectL2", "subObjectL3", "intValDeep" }, 1);
        p.put({ "subObjectL1", "subObjectL2", "subObjectL3", "boolValDeep" }, true);
        p.put({ "subObjectL1", "subObjectL2", "subObjectL3", "strValDeep" }, "string");
        p.put({ "subObjectL1", "subObjectL2", "subObjectL3", "arrayValDeep" }, std::vector<int>{1, 2, 3, 4});


        bool hasKeyPath = p.contains({ "subObjectL1", "subObjectL2", "subObjectL3" });
        auto hasKey = p.searchKeyName("strValDeep");
        p.put({ "subObjectL1", "subObjectL2", "strValDeep" }, "string");
        p.put({ "subObjectL1", "subObjectL2",  "subObjectL3", "subObjectL4","strValDeep" }, "string");
        hasKey = p.searchKeyName("strValDeep");
        int         goodGetint;
        bool        goodGetBool;
        std::string goodGetStr;
        std::vector<int> goodGetVec;

        goodGetint  = p.get({ "intVal" }, 0);
        goodGetBool = p.get({ "boolVal" }, false);
        goodGetStr  = p.get({ "strVal" }, std::string("default"));
        goodGetVec  = p.get({ "arrayVal" }, std::vector<int>{});

        goodGetint  = p.get({ "bad_intVal" }, 0);
        goodGetBool = p.get({ "bad_boolVal" }, false);
        goodGetStr  = p.get({ "bad_strVal" }, std::string("default"));
        goodGetVec  = p.get({ "bad_arrayVal" }, std::vector<int>{});

        ret = p.save();
    }
    catch (std::exception& err)
    {
        std::cout << "Error: " << err.what();
    }
}
