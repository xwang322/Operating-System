from tempfile import mktemp

import toolspath
from testing import Test

# tests for checking correct error behavior

usage_error = "Usage: rangesort -i inputfile -o outputfile -l lowvalue -h highvalue\n"
invalid_range_error = "Error: Invalid range value\n"

class Arg(Test):
   name = "arg"
   description = "Bad arguments"
   point_value = 1
   timeout = 5
   def run(self):
      self.runexe(["rangesort"],
            stderr = usage_error, status = 1)
      self.done()

class Arg2(Test):
   name = "arg2"
   description = "Bad arguments"
   point_value = 1
   timeout = 5
   def run(self):
      self.runexe(["rangesort", "a", "b", "c", "d"],
            stderr = usage_error, status = 1)
      self.done()

class Arg3(Test):
   name = "arg3"
   description = "Missing flag"
   point_value = 1
   timeout = 5
   def run(self):
      self.runexe(["rangesort", "-i", "input", "-o", "output", "-h", "10"],
            stderr = usage_error, status = 1)
      self.done()

class Arg4(Test):
   name = "arg4"
   description = "Undefined extra flag"
   point_value = 1
   timeout = 5
   def run(self):
      self.runexe(["rangesort", "-i", "input", "-o", "output", "-l", "0", "-h", "8", "-k", "invalid"],
            stderr = usage_error, status = 1)
      self.done()

class Arg5(Test):
   name = "arg5"
   description = "Invalid range error: a non 32-bit unsigned integer value given as low value"
   point_value = 1
   timeout = 5
   def run(self):
      self.runexe(["rangesort", "-i", "input", "-o", "output", "-l", "invalid", "-h", "10"],
            stderr = invalid_range_error, status = 1)
      self.done()

class Arg6(Test):
   name = "arg6"
   description = "Invalid range error: a non 32-bit unsigned integer value given as high value"
   point_value = 1
   timeout = 5
   def run(self):
      self.runexe(["rangesort", "-i", "input", "-o", "output", "-l", "5", "-h", "invalid"],
            stderr = invalid_range_error, status = 1)
      self.done()

class Badin(Test):
   name = "badin"
   description = "Bad input file"
   point_value = 1
   timeout = 5
   def run(self):
      "mktemp --> mkstemp"
      invalid = mktemp(prefix='/invalid/path/')
      self.runexe(["rangesort", "-i", invalid, "-o", "outfile", "-l", "1", "-h", "100"],
          stderr = "Error: Cannot open file {0}\n".format(invalid), status = 1)
      self.done()

class Badout(Test):
   name = "badout"
   description = "Bad output file"
   point_value = 1
   timeout = 5
   def run(self):
      infile = self.project_path + "/infile"
      # create a valid (empty) input file
      open(infile, "a").close()
      "mktemp --> mkstemp"
      invalid = mktemp(prefix='/invalid/path/')
      self.runexe(["rangesort", "-i", infile, "-o", invalid, "-l", "1", "-h", "100"],
          stderr = "Error: Cannot open file {0}\n".format(invalid), status = 1)
      self.done()

class InvalidHighValue(Test):
   name = "invalidhighvalue"
   description = "Invalid range error: High value greater than UINT_MAX"
   point_value = 1
   timeout = 5
   def run(self):
      infile = self.project_path + "/infile"
      low = str(0)
      high = str(9999999999999)
      open(infile, "a").close()
      self.runexe(["rangesort", "-i", infile, "-o", "outfile", "-l", low, "-h", high],
          stderr = invalid_range_error, status = 1)
      self.done()

class InvalidLowValue(Test):
   name = "invalidlowvalue"
   description = "Invalid range error: Negative low value"
   point_value = 1
   timeout = 5
   def run(self):
      infile = self.project_path + "/infile"
      low = str(-5)
      high = str(999)   
      open(infile, "a").close()
      self.runexe(["rangesort", "-i", infile, "-o", "outfile", "-l", low, "-h", high],
          stderr = invalid_range_error, status = 1)
      self.done()

class InvalidValues_1(Test):
   name = "invalidvalues_1"
   description = "Invalid range error: Low and high values greater than UINT_MAX"
   point_value = 1
   timeout = 5
   def run(self):
      infile = self.project_path + "/infile"
      low = str(8888888888888)
      high = str(9999999999999)   
      open(infile, "a").close()
      self.runexe(["rangesort", "-i", infile, "-o", "outfile", "-l", low, "-h", high],
          stderr = invalid_range_error, status = 1)
      self.done()

class InvalidValues_2(Test):
   name = "invalidvalues_2"
   description = "Invalid range error: Negative low and high value"
   point_value = 1
   timeout = 5
   def run(self):
      infile = self.project_path + "/infile"
      low = str(-5)
      high = str(-1)   
      open(infile, "a").close()
      self.runexe(["rangesort", "-i", infile, "-o", "outfile", "-l", low, "-h", high],
          stderr = invalid_range_error, status = 1)
      self.done()

class LowGreaterHigh(Test):
   name = "lowgreaterhigh"
   description = "Low value greater than the high value"
   point_value = 1
   timeout = 5
   def run(self):
      infile = self.project_path + "/infile"
      low = str(1000)
      high = str(500)   
      open(infile, "a").close()
      self.runexe(["rangesort", "-i", infile, "-o", "outfile", "-l", low, "-h", high],
          stderr = invalid_range_error, status = 1)
      self.done()

test_list = [Arg, Arg2, Arg3, Arg4, Arg5, Arg6,  Badin, Badout, InvalidHighValue, InvalidLowValue, InvalidValues_1, InvalidValues_2, LowGreaterHigh]
