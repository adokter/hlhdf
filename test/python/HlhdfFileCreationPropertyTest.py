'''
Created on Oct 12, 2009

@author: anders
'''
import unittest
import _pyhl

class HlhdfFileCreationPropertyTest(unittest.TestCase):
  
  def setUp(self):
    _pyhl.show_hlhdferrors(0)
    _pyhl.show_hdf5errors(0)      
  
  def tearDown(self):
    pass
  
  def testSetSizes_valid(self):
    VALID_VALUES = [(0,0), (2,2), (4,4), (8,8), (16,16)]
    fcp = _pyhl.filecreationproperty()
    for t in VALID_VALUES:
      fcp.sizes = t
      self.assertEqual(t, fcp.sizes)
      
  def testSetSizes_invalid(self):
    VALID_VALUES = [(0,1), (1,0), (3,3), (4,3), (32,32)]
    fcp = _pyhl.filecreationproperty()
    curr = fcp.sizes
    for t in VALID_VALUES:
      try:
        fcp.sizes = t
        self.fail("Expected AttributeError")
      except AttributeError:
        pass
      self.assertEqual(curr, fcp.sizes)

    