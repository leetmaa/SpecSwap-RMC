#include <cppunit/ui/text/TestRunner.h>
#include "testRunner.h"

int main ()
{
  CppUnit::Test *test =
    CppUnit::TestFactoryRegistry::getRegistry().makeTest();
  CppUnit::TextTestRunner runner;
  runner.addTest(test);

  runner.run();
  return 0;
}

