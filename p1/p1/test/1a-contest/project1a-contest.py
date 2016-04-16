from fsbuild import RangesortBuild
tester_files = ["contest"]

all = []
for f in tester_files:
  module = __import__(f)
  all.extend(module.test_list)

import toolspath
from testing.runtests import main
main(RangesortBuild, all)
