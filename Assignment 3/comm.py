#!/usr/bin/python

"""
Output lines selected randomly from a file

Copyright 2005, 2007 Paul Eggert.
Copyright 2010 Darrell Benjamin Carbajal.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $
"""

import sys, string, locale
locale.setlocale(locale.LC_ALL, '')

from optparse import OptionParser

class comm:
    def __init__(self, filename1, filename2):
        if filename1 == '-':
            self.lines1 = sys.stdin.readlines()
        else:
            f1 = open(filename1, 'r')
            self.lines1 = f1.readlines()
            f1.close()

        if filename2 == '-':
            self.lines2 = sys.stdin.readlines()
        else:
            f2 = open(filename2, 'r')
            self.lines2 = f2.readlines()
            f2.close()

    def testSorted1(self):
        i = 0;
        while i < len(self.lines1):
            if i > 0:
                a = i - 1
                if locale.strcoll(self.lines1[i],self.lines1[a]) < 0:
                    return False
            i = i + 1
        return True

    def testSorted2(self):
        i = 0;
        while i< len(self.lines2):
            if i > 0:
                a = i - 1
                if locale.strcoll(self.lines2[i],self.lines2[a]) < 0:
                    return False
            i =i + 1
        return True

    def printCol(self, item, col, suppress1, suppress2, suppress3):
    #  Attempt to print the item in the first col
        if col == 1:
            if suppress1:
                return
            else:
                sys.stdout.write("{0}".format(item))
        if col == 2:
            if suppress2:
                return
            if suppress1:
                sys.stdout.write("{0}".format(item))
            elif not suppress1:
                sys.stdout.write("{0}{1}".format("        ", item))
        if col == 3:
            if suppress3:
                return
            elif suppress1 and (not suppress2):
                sys.stdout.write("{0}{1}".format("        ", item))
            elif (not suppress1) and suppress2:
                sys.stdout.write("{0}{1}".format("        ", item))
            elif suppress1 and suppress2:
                sys.stdout.write("{0}".format(item))
            else:
                sys.stdout.write("                {0}".format(item))
        
    def compareSorted(self, suppress1, suppress2, suppress3):
        i = 0
        j = 0
        while i < len(self.lines1) and j < len(self.lines2):
            a = 0
            b = 0
        #  if the line from FILE1 collates before FILE2, we print the line
        #     from FILE1 and move to the next line in FILE1
            if locale.strcoll(self.lines1[i], self.lines2[j]) < 0:
                self.printCol(self.lines1[i], 1, suppress1, suppress2,
                              suppress3)
                i = i + 1
            elif locale.strcoll(self.lines1[i], self.lines2[j]) > 0:
                self.printCol(self.lines2[j], 2, suppress1, suppress2,
                              suppress3)
                j = j + 1
            else:
                self.printCol(self.lines2[j], 3, suppress1, suppress2,
                              suppress3)
                i = i + 1
                j = j + 1

        while i < len(self.lines1):
            self.printCol(self.lines1[i], 1, suppress1, suppress2, suppress3)
            i = i + 1
        while j < len(self.lines2):
            self.printCol(self.lines2[j], 2, suppress1, suppress2, suppress3)
            j = j + 1

    def compareUnsorted(self, suppress1, suppress2, suppress3):
        tempLines1 = []
        tempLines1 = self.lines1
        len1 = len(tempLines1)
        tempLines1[len1-1]=((tempLines1[len1-1]).strip('\n')+"\n")
        tempLines2 = []
        tempLines2 = self.lines2
        len2 = len(tempLines2)
        tempLines2[len2-1]=((tempLines2[len2-1]).strip('\n')+"\n")

        for i in tempLines1:
            if i in tempLines2:
                if suppress3 == False:
                    sys.stdout.write("                {0}".format(i))
                tempLines2.remove(i)
            else:
                if suppress1 == False:
                    sys.stdout.write("{0}".format(i))
                    
        for k in tempLines2:
            self.printCol(k, 2, suppress1, suppress2, suppress3)            
        
def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2
Compare sorted files FILE1 and FILE2 line by line."""
    msg = "suppress column 3 (lines that appear in both files)"
    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-1",
                      action="store_true", dest="suppress1", default=False,
                      help="suppress column 1 (lines unique to FILE1)")
    parser.add_option("-2",
                      action="store_true", dest="suppress2", default=False,
                      help="suppress column 2 (lines unique to FILE2)")
    parser.add_option("-3",
                      action="store_true", dest="suppress3", default=False,
                      help=msg)
    parser.add_option("-u",
                      action="store_true", dest="unsorted", default=False,
                      help="work even if its inputs are not sorted")
    
    options, args = parser.parse_args(sys.argv[1:])

    try:
        s1=bool(options.suppress1)
        s2=bool(options.suppress2)
        s3=bool(options.suppress3)
        u=bool(options.unsorted)

    except:
        parser.error("invalid option")

    if len(args) != 2:
        parser.error("wrong number of operands")

    input_file1 = args[0]
    input_file2 = args[1]
    
    try:
        generator = comm(input_file1, input_file2)

        if input_file1 != '-' and input_file2 != '-': 
            if not options.unsorted:
                if (generator.testSorted1()==False):
                    print ("file 1 is not in sorted order")
                    quit()
                if (generator.testSorted2()==False):
                    print ("file 2 is not in sorted order")
                    quit()
        
        if not options.unsorted:
            generator.compareSorted(options.suppress1, options.suppress2,
                                    options.suppress3)
        else:
            generator.compareUnsorted(options.suppress1, options.suppress2,
                                      options.suppress3)
            

    except (OSError) as err:
                parser.error("OS error: {0}".format(err))


if __name__ == "__main__":
    main()
