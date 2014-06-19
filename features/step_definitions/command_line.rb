When(/^I run "(.*?)"$/) do |arg1|
  @output = `#{arg1}`
end

Then(/^the command should return the (.*?) "(.*?)"$/) do |arg1,arg2|
  if !@output.include? "#{arg1}: #{arg2}"
    raise "command does not return correct #{arg1}"
  end
end

When(/^I run "(.*?)" on the command line$/) do |arg1|
  @output = `#{arg1}`
end

Then(/^I get a help message with "(.*?)"$/) do |arg1|
  if !@output.include? arg1
    raise "help message does not include \"#{arg1}\""
  end
end
