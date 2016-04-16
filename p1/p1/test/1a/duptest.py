from time import time
import os

import toolspath
from testing.test import *

from check import check_sorted

class DupTest(Test):
   name = "dup"
   description = "duplicate records"
   point_value = 5
   timeout = 15

   def run(self):
      infile = self.test_path + "/dupdata"
      outfile = self.project_path + "/outfile"
      lowValue = 0
      highValue = 0
      self.log("Running rangesort")
      child = self.runexe(["rangesort", "-i", infile, "-o", outfile, "-l", str(lowValue), "-h", str(highValue)],
          status = 0, stderr = "")
      self.log("Rangesort completed in " + str(child.wallclock_time) + " seconds")
      self.log("Checking output")
      (is_sorted, msg) = check_sorted(outfile, infile, lowValue, highValue)
      os.remove(outfile)
      if not is_sorted:
         raise Failure(msg)
      self.done()

test_list = [DupTest]
