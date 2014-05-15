Feature: Asset Id
  In order to track assets
  As any user
  I want to identify assets

  Scenario: Identifying a new asset on the command line
    Given I have an asset called "test_image.exr"
    When I run "c4 id test_image.exr"
    Then I should get the id in base64
    And get the C4 id in base58
    And get the mime-type
    And get the incept datestamp




