from behave import given, when, then, step

@given('we have a BDD tool installed')
def step_impl(context):
    pass

@when('we implement {number:d} tests')
def step_impl(context, number):  # -- NOTE: number is converted into integer
    assert number > 1 or number == 0
    context.tests_count = number

@then('a BDD tool will test it for us!')
def step_impl(context):
    assert context.failed is False
    assert context.tests_count >= 0
