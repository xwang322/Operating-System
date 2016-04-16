import os, inspect

import toolspath
from testing import BuildTest

curdir = os.path.dirname(inspect.getfile(inspect.currentframe()))

class RangesortBuild(BuildTest):
  targets = ["rangesort"]

  def run(self):
    self.clean(self.targets + ["*.o"], required=False)
    if not self.make(self.targets, required=False):
      self.log("Trying to build using gcc")
      status = self.run_util(
          ["gcc", "-O", "-Wall", "-o", "rangesort", "rangesort.c",
           "-I%s"%(curdir,)])
      if status != 0:
        self.fail("gcc returned error " + str(status))
    self.done()
