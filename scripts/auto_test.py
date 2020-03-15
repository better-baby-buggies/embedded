# Autotest needs to:
# 1. Move to test directory
# 2. Build each ino, log results
# 3. If successful, 
# 4. foreach ino
#   a. Upload
#   b. log serial output (probably use a thread for this)
#   c. wait x minutes
# 5. compile log files into one summary log file

import os
import fnmatch
from datetime import datetime
import logging
import subprocess


class AutoTest():
  def __init__(self):

    # Setup our logging
    now = datetime.now()
    current_time = now.strftime("%H-%M-%S_%m-%d-%Y")
    current_time = "scripts/logs/" + current_time +".log"
    logging.basicConfig(filename=current_time, level=logging.DEBUG)
    self.info_log_and_print_string("Starting auto test...")
    
    # Get the ino files
    self.info_log_and_print_string("Getting Files...")
    self.file_list = []
    self.collect_test_file_names()

    # Build each ino
    for ino_filename in self.file_list:
      print(ino_filename)
      args = [".\\scripts\\build.cmd", ".\\tests\\example_module_test", "example_module_test.ino"]
      
      ret_code = subprocess.call(
        args=args
        # stdout=subprocess.PIPE
      )
      
      # Print the filename and return code.
      out = ''.join(map(str, ino_filename)) + " build return was: " + str(ret_code)
      self.info_log_and_print_string(out)
      



  # This function enters the "tests" folder and adds the files matching '*.ino"
  # to the class variable self.file_list
  def collect_test_file_names(self):
    # Get each filename in the directory recursively
    for (_, _, filenames) in os.walk('./tests/'):
      for filename_str in filenames:
        base_string = ""
        base_string += filename_str
        if fnmatch.fnmatch(base_string, '*.ino'):
          # If we have a match
          self.file_list.append(filenames)

    self.info_log_and_print_string("Files in self.file_list:") 
    self.info_log_and_print_string(self.file_list)
      
    
  def build_ino(self):
    pass

  def log_ino(self):
    pass

  def upload_ino(self):
    pass

  def start_serial_output_log_thread(self):
    pass

  def wait_x_minutes(self):
    pass

  def generate_summary_file(self):
    pass

  def info_log_and_print_string(self, input_str):
    logging.info(input_str)
    print(input_str)

  def debug_log_and_print_string(self, input_str):
    logging.debug(input_str)
    print(input_str)





if __name__=="__main__":
  auto_test = AutoTest()
  