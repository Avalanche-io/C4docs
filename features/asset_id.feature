Feature: Asset Id
  In order to track assets
  As any user
  I want to identify assets

  Scenario: Identifying a new asset on the command line
    Given I have a test image "Digital_LAD_512x389.png"
    When I run "bin/C4 Asset id test_data/Digital_LAD_512x389.png"
    And the command should return the address "dZHXjyH6hi9RQiLbO4q1nYPnrGMqc32KXOAETfhUv2g="
    And the command should return the type "image/png"




