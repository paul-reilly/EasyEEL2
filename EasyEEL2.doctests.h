//---------------------  DOCTEST -----------------------
// included within #ifndef DOCTEST_CONFIG_DISABLE guard
// elsewhere.

// need these implemented for EEL2 to compile
void NSEEL_HOSTSTUB_EnterMutex() { }
void NSEEL_HOSTSTUB_LeaveMutex() { }

static 
auto compile(EELVM &vm, WDL_FastString &results)
  -> bool
{
    vm.compileFile(results);
    INFO(results.Get());
    auto ok = results.GetLength() == 0;
    return ok;
}

TEST_CASE("EasyEEL2: compile and execute stringstream") {
    EELVM vm({ "@code", "@numpty" });
    WDL_FastString results;
    std::istringstream code(R"__(@code

a = 42;
printf("This is FUCKING EXECUTING!!!!");

@numpty

a += 1;

)__");

    auto* a = vm.registerVar("a");
    SUBCASE("compilation") {
        vm.compileStream(code, results);
        INFO(results.Get());
        CHECK(results.GetLength() == 0);
        CHECK(vm.getCodeHandlesSize() == 2);
    }
    SUBCASE("execution") {
        CHECK(vm.executeHandle("@code") == true);
        INFO(results.Get());
    }
    *a = 100.;

    SUBCASE("change registered var script") {
        CHECK(vm.executeHandle(1) == true);
        CHECK(*a == 101);
    }
}

TEST_CASE("EEL2 parsing") {
    LineParser lp(true);
    auto is = std::istringstream("@code;\n a = 1; ");
    std::vector<char> buffer(4096, 0);
    is.read(buffer.data(), 4096);
    char* linebuf = buffer.data();
    CHECK(!lp.parse(linebuf));
    CHECK(lp.getnumtokens() > 0);
    CHECK(lp.gettoken_str(0)[0] == '@');
}



TEST_CASE("EasyEEL2: compile file with 3 sections") {
    EELVM vm({"@code", "@bling", "@alwayslast"}, "test-script.eel");
    WDL_FastString results;
    CHECK(compile(vm, results));
    INFO("#codehandles = " << vm.getCodeHandlesSize());
    SUBCASE("number of code handles") {
        CHECK(vm.getCodeHandlesSize() == 3);
    }
    SUBCASE("execute first section") {
        INFO(results.Get());
        CHECK(vm.executeHandle("@code") == true);
    }  
}



TEST_CASE("EasyEEL2: string tests") {
    EELVM vm({"@code"});
    WDL_FastString results;
    std::istringstream code(R"__(@code
a = "U";
)__");
    SUBCASE("compilation") {
        CHECK(vm.compileStream(code, results));
        INFO(results.Get());
        CHECK(results.GetLength() == 0);
    }
    SUBCASE("execution") {
        INFO(results.Get());
        CHECK(vm.executeHandle(0) == true);
    }
}
//*/