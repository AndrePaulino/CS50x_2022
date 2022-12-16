import csv
import sys


def main():
    if len(sys.argv) != 3:
        print("Usage: dna.py database.csv sequence.txt")
        sys.exit(1)

    database = load_database(sys.argv[1])
    sequence = load_sequence(sys.argv[2])
    subsequences = extract_subsequences(database)
    STR_counts = count_STR(sequence, subsequences)

    found = compare_STR_database(STR_counts, database)
    if found:
        print(found)
    else:
        print("No match")


def load_database(database):
    db_array = []
    with open(database) as file:
        db_dict = csv.DictReader(file)
        for person in db_dict:
            for STR in person:
                if STR == "name":
                    continue
                person[STR] = int(person[STR])
            db_array.append(person)

    return db_array


def load_sequence(dna_sequence):
    sequence_STR = ""
    with open(dna_sequence) as file:
        sequence_STR = file.read().strip()

    return sequence_STR


def extract_subsequences(database):
    subsequences = []
    for subsequence in database[0]:
        if subsequence == "name":
            continue
        subsequences.append(subsequence)
    return subsequences


def count_STR(sequence, subsequences):
    STR_counts = {}
    for subsequence in subsequences:
        STR_counts[subsequence] = longest_match(sequence, subsequence)  # noqa: E501

    return STR_counts


def compare_STR_database(STR_count, database):
    for person in database:
        STR_match = len(STR_count)
        for STR in person:
            if STR == "name":
                continue
            if person[STR] != STR_count[STR]:
                break
            STR_match -= 1

        if STR_match == 0:
            return person["name"]


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence # noqa: E501
        # If a match, move str to next potential match in sequence
        # Continue moving subSTRing and checking for matches until out of consecutive matches # noqa: E501
        while True:

            # Adjust subSTRing start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found # noqa: E501
    return longest_run


if __name__ == "__main__":
    main()
