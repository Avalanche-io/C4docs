Given(/^I have a test image "(.*?)"$/) do |arg1|
  if !File.file?("test_data/#{arg1}")
    raise "test_data/#{arg1} not found"
  end
end
