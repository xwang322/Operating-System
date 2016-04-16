import os

import toolspath
from testing.test import *

from check import check_sorted

# tests that use the generate utility and then check the output

class GenTest(Test):
   seed = 0
   n = 0
   lowValue = 0
   highValue = 0
   def run(self):
      generate_path = self.test_path + "/generate"
      infile = self.project_path + "/infile"
      outfile = self.project_path + "/outfile"

      self.log("Generating input file")
      status = self.run_util([generate_path, "-s", str(self.seed),
         "-n", str(self.n), "-o", infile])
      if status != 0:
         raise Exception("generate failed with error " + str(status))

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

class Empty(GenTest):
   name = "empty"
   description = "Empty input file. Output file should be empty"
   timeout = 15
   point_value = 7
   seed = 10
   n = 0
   lowValue = 918
   highValue = 7750

class One(GenTest):
   name = "one"
   description = "Input file with a single record"
   timeout = 15
   point_value = 7
   seed = 10
   n = 1
   lowValue = 2 
   highValue = 1215069295

class Five1(GenTest):
   name = "five1"
   description = "Input file with five records"
   timeout = 15
   point_value = 7
   seed = 11
   n = 5
   lowValue = 758693
   highValue = 49389750

class Five2(GenTest):
   name = "five2"
   description = "Input file with five records"
   timeout = 15
   point_value = 7
   seed = 12
   n = 5
   lowValue = 3175869
   highValue = 11897750

class Medium(GenTest):
   name = "medium"
   description = "Input file with 100 records"
   timeout = 20
   point_value = 7
   seed = 13
   n = 100
   lowValue = 30029984
   highValue = 100948125

class Large(GenTest):
   name = "large"
   description = "Input file with 10k records"
   timeout = 30
   point_value = 7
   seed = 14
   n = 10000
   lowValue = 2000
   highValue = 1246803579

class VeryLarge(GenTest):
   name = "verylarge"
   description = "Input file with 100k records"
   timeout = 60
   point_value = 7
   seed = 15
   n = 100000
   lowValue = 100
   highValue = 2000000000


test_list = [Empty, One, Five1, Five2, Medium, Large, VeryLarge]
