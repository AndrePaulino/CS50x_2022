from cs50 import get_string
import re


def main():
    text = get_string("Text: ")

    grade = coleman_liau_score(text)

    if 1 <= grade < 16:
        print(f"Grade: {grade}")
    elif grade > 16:
        print("Grade 16+")
    else:
        print("Before Grade 1")


def coleman_liau_score(text):
    sentence_regex = re.compile("\\.+|\\!+|\\?+")
    word_regex = re.compile("\\s")
    letter_regex = re.compile("\\w")

    sentences_total = len(re.findall(sentence_regex, text))
    words_total = len(re.findall(word_regex, text)) + 1
    letters_total = len(re.findall(letter_regex, text))

    l_var = (letters_total / words_total) * 100
    s_var = (sentences_total / words_total) * 100

    index = 0.0588 * l_var - 0.296 * s_var - 15.8

    return round(index)


if __name__ == "__main__":
    main()
