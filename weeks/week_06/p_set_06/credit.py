from cs50 import get_string
import re
import sys


def main():
    card = get_string("Number: ")

    card_company = match_digits_length(card)
    if card_company:
        if validate_card_digits(card):
            print(card_company)
            sys.exit(0)

    print("INVALID")
    sys.exit(1)


card_companies = [
    {
        "name": "VISA",
        "regex": re.compile("^4\\d{12}$")
    },
    {
        "name": "VISA",
        "regex": re.compile("^4\\d{15}$")
    },
    {
        "name": "AMEX",
        "regex": re.compile("^34|37\\d{13}$")
    },
    {
        "name": "MASTERCARD",
        "regex": re.compile("^5[1-5]\\d{14}$")
    },


]


def match_digits_length(card):
    for company in card_companies:
        card_match = re.search(company['regex'], card)
        if card_match:
            return company['name']

    return False


def validate_card_digits(card_number):
    card = int(card_number)
    digit, counter, digit_sum = 0, 0, 0

    while card:
        # Get single digit counting backwards
        digit = card % 10

        # Using a counter, get every other one, from the second and double it
        if counter % 2 != 0:
            digit *= 2

        # Sum the digits
        digit_sum += digit % 10
        digit_sum += digit // 10

        # Remove that last digit and count up
        card //= 10
        counter += 1

    if digit_sum % 10 == 0:
        return True

    return False


if __name__ == "__main__":
    main()
