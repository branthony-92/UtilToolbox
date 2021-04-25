#include <stdafx.h>
#include "PersistorTest.h"


TEST_F(PersistorTest, BasicItemManipulationPutAndGet)
{
    auto p = pers();
    p->setName("Basic_Item_Manipulation");
    
    // put and retrieve values in the JSON persistence object
    const int               c_testInt = 1;
    const bool              c_testBool = true;
    const std::string       c_testString = "string";
    const std::vector <int> c_testtVec = { 0, 1, 2, 3 };

    EXPECT_TRUE(p->put({ "intVal" },   c_testInt));
    EXPECT_TRUE(p->put({ "boolVal" },  c_testBool));
    EXPECT_TRUE(p->put({ "strVal" },   c_testString));
    EXPECT_TRUE(p->put({ "arrayVal" }, c_testtVec));

    const int               c_defaultInt = 0;
    const bool              c_defaultBool = false;
    const std::string       c_defaultString = "default";
    const std::vector <int> c_defaultVec = {};

    auto goodGetint = p->get({ "intVal" }, c_defaultInt);
    EXPECT_EQ(goodGetint, c_testInt);
    EXPECT_NE(goodGetint, c_defaultInt);

    auto goodGetBool = p->get({ "boolVal" }, c_defaultBool);
    EXPECT_EQ(goodGetBool, c_testBool);
    EXPECT_NE(goodGetBool, c_defaultBool);

    auto goodGetStr = p->get({ "strVal" }, c_defaultString);
    EXPECT_EQ(goodGetStr, c_testString);
    EXPECT_NE(goodGetStr, c_defaultString);

    auto goodGetVec = p->get({ "arrayVal" }, c_defaultVec);
    EXPECT_TRUE(!goodGetVec.empty());
    if (!goodGetVec.empty())
    {
        for (auto i = 0u; i < goodGetVec.size(); i++)
        {
            EXPECT_EQ(goodGetVec[i], c_testtVec[i]);
        }
    }
}

TEST_F(PersistorTest, BasicItemManipulationGetNonExistant)
{
    auto p = pers();
    p->setName("Basic_Item_Manipulation");

    const int               c_defaultInt = 0;
    const bool              c_defaultBool = false;
    const std::string       c_defaultString = "default";
    const std::vector <int> c_defaultVec = {};

    // Attempt to retrieve a key that does not exist, should return the supplied default
    auto goodGetint = p->get({ "intVal" }, c_defaultInt);
    EXPECT_EQ(goodGetint, c_defaultInt);

    auto goodGetBool = p->get({ "boolVal" }, c_defaultBool);
    EXPECT_EQ(goodGetBool, c_defaultBool);

    auto goodGetStr = p->get({ "strVal" }, c_defaultString);
    EXPECT_EQ(goodGetStr, c_defaultString);

    auto goodGetVec = p->get({ "arrayVal" }, c_defaultVec);
    EXPECT_TRUE(goodGetVec.empty());
}

TEST_F(PersistorTest, SubObjectManipulationPutAndGet)
{
    auto p = pers();
    p->setName("SubObject_Item_Manipulation");

    const int               c_testInt = 1;
    const bool              c_testBool = true;
    const std::string       c_testString = "string";
    const std::vector <int> c_testtVec = { 0, 1, 2, 3 };

    // Add 4 values to a single 3 layer deep subkey
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "intVal" },   c_testInt));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "boolVal" },  c_testBool));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "strVal" },   c_testString));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "arrayVal" }, c_testtVec));

    const int               c_defaultInt = 0;
    const bool              c_defaultBool = false;
    const std::string       c_defaultString = "default";
    const std::vector <int> c_defaultVec = {};

    // Attempt to recover the values given the subkey paths
    auto goodGetint = p->get({ "SubObject1", "SubObject2", "SubObject3", "intVal" }, c_defaultInt);
    EXPECT_EQ(goodGetint, c_testInt);
    EXPECT_NE(goodGetint, c_defaultInt);

    auto goodGetBool = p->get({ "SubObject1", "SubObject2", "SubObject3", "boolVal" }, c_defaultBool);
    EXPECT_EQ(goodGetBool, c_testBool);
    EXPECT_NE(goodGetBool, c_defaultBool);

    auto goodGetStr = p->get({ "SubObject1", "SubObject2", "SubObject3", "strVal" }, c_defaultString);
    EXPECT_EQ(goodGetStr, c_testString);
    EXPECT_NE(goodGetStr, c_defaultString);

    auto goodGetVec = p->get({ "SubObject1", "SubObject2", "SubObject3", "arrayVal" }, c_defaultVec);
    EXPECT_TRUE(!goodGetVec.empty());
    if (!goodGetVec.empty())
    {
        for (auto i = 0u; i < goodGetVec.size(); i++)
        {
            EXPECT_EQ(goodGetVec[i], c_testtVec[i]); // validate our vector entries
        }
    }
}

TEST_F(PersistorTest, SearchKeyNameHits)
{
    auto p = pers();
    p->setName("Item_Search_For_Hits");

    const int               c_testInt = 1;
    const bool              c_testBool = true;
    const std::string       c_testString = "string";
    const std::vector <int> c_testtVec = { 0, 1, 2, 3 };

    // Add several levels of subkeys, each containing the same 4 keys
    EXPECT_TRUE(p->put({ "SubObject1", "intVal"   }, c_testInt));
    EXPECT_TRUE(p->put({ "SubObject1", "boolVal"  }, c_testBool));
    EXPECT_TRUE(p->put({ "SubObject1", "strVal"   }, c_testString));
    EXPECT_TRUE(p->put({ "SubObject1", "arrayVal" }, c_testtVec));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "intVal"   }, c_testInt));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "boolVal"  }, c_testBool));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "strVal"   }, c_testString));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "arrayVal" }, c_testtVec));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "intVal"   }, c_testInt));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "boolVal"  }, c_testBool));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "strVal"   }, c_testString));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "arrayVal" }, c_testtVec));

    // Search for the total number of ocurrances of the given key name
    EXPECT_EQ(p->searchKeyName("SubObject1"), 1); // dir name only appears once
    EXPECT_EQ(p->searchKeyName("SubObject2"), 1); // dir name only appears once
    EXPECT_EQ(p->searchKeyName("SubObject3"), 1); // dir name only appears once

    EXPECT_EQ(p->searchKeyName("intVal"),   3); // exists in each subdir, should return 1
    EXPECT_EQ(p->searchKeyName("boolVal"),  3); // exists in each subdir, should return 1
    EXPECT_EQ(p->searchKeyName("strVal"),   3); // exists in each subdir, should return 1
    EXPECT_EQ(p->searchKeyName("arrayVal"), 3); // exists in each subdir, should return 1

    EXPECT_EQ(p->searchKeyName("NO_KEY_HERE"), 0); // does not exist in any subdir, shoud return 0
}

TEST_F(PersistorTest, FindKeyWithPath)
{
    auto p = pers();
    p->setName("Item_Search_With_Path");

    const int               c_testInt = 1;
    const bool              c_testBool = true;
    const std::string       c_testString = "string";
    const std::vector <int> c_testtVec = { 0, 1, 2, 3 };

    // add a value in a deep chain of subkeys
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "intVal" }, c_testInt));

    // use the known subkey path to verify that we have  the given key
    EXPECT_TRUE(p->contains({ "SubObject1", "SubObject2", "SubObject3", "intVal" }));  // good 
    EXPECT_FALSE(p->contains({ "SubObject1", "SubObject2", "intVal" }));               // bad, wrong sub object
    EXPECT_FALSE(p->contains({ "SubObject1", "SubObject2", "SubObject3", "IntVal" })); // bad, wrong key name
}

TEST_F(PersistorTest, SaveFile)
{
    auto p = pers();
    p->setName("File_Save_Test");

    const int               c_testInt = 1;
    const bool              c_testBool = true;
    const std::string       c_testString = "string";
    const std::vector <int> c_testtVec = { 0, 1, 2, 3 };

    // add some entries to the persistor
    EXPECT_TRUE(p->put({ "SubObject1", "intVal" }, c_testInt));
    EXPECT_TRUE(p->put({ "SubObject1", "boolVal" }, c_testBool));
    EXPECT_TRUE(p->put({ "SubObject1", "strVal" }, c_testString));
    EXPECT_TRUE(p->put({ "SubObject1", "arrayVal" }, c_testtVec));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "intVal" }, c_testInt));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "boolVal" }, c_testBool));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "strVal" }, c_testString));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "arrayVal" }, c_testtVec));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "intVal" }, c_testInt));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "boolVal" }, c_testBool));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "strVal" }, c_testString));
    EXPECT_TRUE(p->put({ "SubObject1", "SubObject2", "SubObject3", "arrayVal" }, c_testtVec));

    // attempt to save the file TODO : validate in file system
    EXPECT_EQ(p->save(), CPersistor::ReturnStatus::OK);
}

TEST_F(PersistorTest, LoadFile)
{
    auto p = pers();
    p->setName("File_Save_Test");

    // attempt to the load the file
    auto ret = p->load();

    EXPECT_TRUE(ret == CPersistor::ReturnStatus::OK || ret == CPersistor::ReturnStatus::NoFile);

    // If the file doesn't exist skip this part
    if (ret == CPersistor::ReturnStatus::NoFile) return;

    // validate based on data in the Save test
    const int               c_defaultInt = 0;
    const bool              c_defaultBool = false;
    const std::string       c_defaultString = "default";
    const std::vector <int> c_defaultVec = {};

    const int               c_testInt = 1;
    const bool              c_testBool = true;
    const std::string       c_testString = "string";
    const std::vector <int> c_testtVec = { 0, 1, 2, 3 };

    EXPECT_EQ(p->get({ "SubObject1", "intVal" },  c_defaultInt),    c_testInt);
    EXPECT_EQ(p->get({ "SubObject1", "boolVal" }, c_defaultBool),   c_testBool);
    EXPECT_EQ(p->get({ "SubObject1", "strVal" },  c_defaultString), c_testString);

    EXPECT_EQ(p->get({ "SubObject1", "SubObject2", "intVal" },  c_defaultInt),    c_testInt);
    EXPECT_EQ(p->get({ "SubObject1", "SubObject2", "boolVal" }, c_defaultBool),   c_testBool);
    EXPECT_EQ(p->get({ "SubObject1", "SubObject2", "strVal" },  c_defaultString), c_testString);

    EXPECT_EQ(p->get({ "SubObject1", "SubObject2", "SubObject3", "intVal" },  c_defaultInt),    c_testInt);
    EXPECT_EQ(p->get({ "SubObject1", "SubObject2", "SubObject3", "boolVal" }, c_defaultBool),   c_testBool);
    EXPECT_EQ(p->get({ "SubObject1", "SubObject2", "SubObject3", "strVal" },  c_defaultString), c_testString);

    std::vector<int> vec = c_defaultVec;

    vec = (p->get({ "SubObject1", "arrayVal" }, c_defaultVec));
    EXPECT_FALSE(vec.empty());
    if (!vec.empty())
    {
        for (auto i = 0u; i < vec.size(); i++)
        {
            EXPECT_EQ(vec[i], c_testtVec[i]);
        }
    }

    vec = (p->get({ "SubObject1", "SubObject2", "arrayVal" }, c_defaultVec));
    EXPECT_FALSE(vec.empty());
    if (!vec.empty())
    {
        for (auto i = 0u; i < vec.size(); i++)
        {
            EXPECT_EQ(vec[i], c_testtVec[i]);
        }
    }

    vec = (p->get({ "SubObject1", "SubObject2", "SubObject3", "arrayVal" }, c_defaultVec));
    EXPECT_FALSE(vec.empty());
    if (!vec.empty())
    {
        for (auto i = 0u; i < vec.size(); i++)
        {
            EXPECT_EQ(vec[i], c_testtVec[i]);
        }
    }
}