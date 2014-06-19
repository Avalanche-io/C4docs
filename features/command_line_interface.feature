Feature: Command line interface
  In order to use the features of the C4 interface from the command line
  As any user on mac, pc, or linux
  I want to have a command line tool

  Scenario: C4 command
    When I run "bin/C4" on the command line
    Then I get a help message with "Usage"



