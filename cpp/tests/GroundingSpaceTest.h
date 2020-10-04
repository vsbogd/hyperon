#include <cxxtest/TestSuite.h>

#include <hyperon/logger.h>
#include <hyperon/GroundingSpace.h>

#include "common.h"
#include "GroundedArithmetic.h"

void add_factorial_definition(GroundingSpace& kb) {
    kb.add_atom(E({ S("="), E({ S("f"), Int(0) }), Int(1) }));
    kb.add_atom(E({ S("="),
                E({ S("f"), V("n") }),
                E({ MUL,
                        E({ S("f"), E({ SUB, V("n"), Int(1) }) }),
                        V("n") }) }));
}

class GroundingSpaceTest : public CxxTest::TestSuite {
public:

    void test_expr_atom_to_string() {
        AtomPtr atom = E({S("="), V("a"), S("0")});
        TS_ASSERT_EQUALS(atom->to_string(), "(= $a 0)");
    }

    void test_expr_atom_equals() {
        AtomPtr atom = E({S("="), V("a"), S("0")});
        TS_ASSERT(*atom == *E({S("="), V("a"), S("0")}));
    }

    void test_match_function_definition() {
        GroundingSpace kb;
        kb.add_atom(E({ S(":-"), E({ S("f"), S("0") }), S("1") }));
        kb.add_atom(E({ S(":-"), E({ S("f"), V("n") }),
                    E({ S("*"), V("n"), E({ S("-"), V("n"), S("1") }) }) }));
        GroundingSpace pattern;
        pattern.add_atom(E({ S(":-"), E({ S("f"), S("5") }), V("x") }));
        GroundingSpace templ;
        templ.add_atom(V("x"));

        GroundingSpace result;
        kb.match(pattern, templ, result);

        GroundingSpace expected;
        expected.add_atom(E({ S("*"), S("5"), E({ S("-"), S("5"), S("1") }) }));
        TS_ASSERT(expected == result);
    }

    void test_match_data() {
        GroundingSpace kb;
        kb.add_atom(E({ S("isa"), S("kitchen-lamp"), S("lamp") }));
        kb.add_atom(E({ S("isa"), S("bedroom-lamp"), S("lamp") }));
        GroundingSpace pattern;
        pattern.add_atom(E({ S("isa"), V("x"), S("lamp") }));
        GroundingSpace templ;
        templ.add_atom(V("x"));

        GroundingSpace result;
        kb.match(pattern, templ, result);

        GroundingSpace expected;
        expected.add_atom(S("kitchen-lamp"));
        expected.add_atom(S("bedroom-lamp"));
        TS_ASSERT(expected == result);
    }

    void test_interpret_plain_expr() {
        Logger::setLevel(Logger::TRACE);
        GroundingSpace kb;
        add_factorial_definition(kb);
        GroundingSpace target;
        target.add_atom(E({ S("f"), Int(5) }));

        AtomPtr result = interpret_until_result(target, kb);

        TS_ASSERT(*Int(120) == *result);
    }

    void test_interpret_plain_expr_twice() {
        Logger::setLevel(Logger::TRACE);
        GroundingSpace kb;
        add_factorial_definition(kb);
        GroundingSpace target;
        target.add_atom(E({ S("f"), Int(3) }));
        target.add_atom(E({ S("f"), Int(5) }));

        AtomPtr result = interpret_until_result(target, kb);
        TS_ASSERT(*Int(120) == *result);

        result = interpret_until_result(target, kb);
        TS_ASSERT(*Int(6) == *result);

    }

    void test_match_variable_in_target() {
        Logger::setLevel(Logger::TRACE);
        GroundingSpace kb;
        kb.add_atom(E({ S("="), E({ S("isa"), S("Fred"), S("frog") }),
                    S("True") }));
        GroundingSpace target;
        target.add_atom(E({ S("isa"), S("Fred"), V("x") }));

        AtomPtr result = interpret_until_result(target, kb);

        TS_ASSERT(*S("True") == *result);
    }
};
