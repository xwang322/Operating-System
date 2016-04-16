import os

import toolspath
from testing.test import *

from check import check_sorted

# tests that use the generate utility and then check the output

class SelectedTest(Test):
   lowValue = 0
   highValue = 0

   def run(self):
      #infile = self.project_path + "/selectedrecs"
      infile = self.test_path + "/selectedrecs"
      outfile = self.project_path + "/outfile"

      self.log("Running rangesort")
      child = self.runexe(["rangesort", "-i", infile, "-o", outfile, "-l", str(self.lowValue), "-h", str(self.highValue)],
            status = 0, stderr = "")
      self.log("Rangesort completed in " + str(child.wallclock_time) +
            " seconds")

      self.log("Checking output")
      (is_sorted, msg) = check_sorted(outfile, infile, self.lowValue, self.highValue)
      os.remove(outfile)
      if not is_sorted:
         raise Failure(msg)
      self.done()

class ZeroRec(SelectedTest):
   name = "zero selected"
   description = "Zero records sorted"
   timeout = 15
   point_value = 5
   lowValue = 10
   highValue = 200

class OneRec(SelectedTest):
   name = "one selected"
   description = "One record sorted"
   timeout = 15
   point_value = 5
   lowValue = 460457228
   highValue = 460457228

class AllRecs(SelectedTest):
   name = "all selected"
   description = "All records sorted"
   point_value = 5
   timeout = 15
   lowValue = 0
   highValue = 1482533822

test_list = [ZeroRec, OneRec, AllRecs]
