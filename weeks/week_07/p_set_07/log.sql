-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Check the crime_scene_report of our crime
SELECT description
FROM crime_scene_reports
WHERE street = 'Humphrey Street'
    AND year = 2021
    AND month = 7
    AND day = 28;
-- Find interviews with the three witnesses
SELECT transcript
FROM interviews
WHERE transcript LIKE "%bakery%"
    AND year = 2021
    AND month = 7
    AND day = 28;
-- "Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame." -> List of license_plate
SELECT license_plate,
    activity
FROM bakery_security_logs
WHERE minute BETWEEN 15 AND 25
    AND hour = 10
    AND year = 2021
    AND month = 7
    AND day = 28;
-- "I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money." -> List of account_number
SELECT account_number
FROM atm_transactions
WHERE atm_location = "Leggett Street"
    AND transaction_type = "withdraw"
    AND year = 2021
    AND month = 7
    AND day = 28;
-- "As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket." -> List of caller, receiver
SELECT caller,
    receiver
FROM phone_calls
WHERE duration < 60
    AND year = 2021
    AND month = 7
    AND day = 28;
-- Destination of first flight leaving the city next day -> New York City
SELECT city
FROM airports
WHERE id IN (
        SELECT destination_airport_id
        FROM flights,
            airports
        WHERE city = "Fiftyville"
            AND year = 2021
            AND month = 7
            AND day = 29
        ORDER BY hour,
            minute
        LIMIT 1
    );
-- NYC flight id -> 36
SELECT flights.id
FROM flights,
    airports
WHERE city = "Fiftyville"
    AND year = 2021
    AND month = 7
    AND day = 29
ORDER BY hour,
    minute
LIMIT 1;
-- Who withdrew money before the theft, left after and was on first flight next day -> Bruce
SELECT name,
    phone_number
FROM atm_transactions,
    people,
    bank_accounts
WHERE atm_location = "Leggett Street"
    AND transaction_type = "withdraw"
    AND year = 2021
    AND month = 7
    AND day = 28
    AND bank_accounts.person_id = people.id
    AND atm_transactions.account_number = bank_accounts.account_number
    AND license_plate IN (
        SELECT license_plate
        FROM bakery_security_logs
        WHERE minute BETWEEN 15 AND 25
            AND hour = 10
            AND year = 2021
            AND month = 7
            AND day = 28
    )
    AND phone_number IN (
        SELECT caller
        FROM phone_calls
        WHERE duration < 60
            AND year = 2021
            AND month = 7
            AND day = 28
    )
    AND passport_number IN (
        SELECT passport_number
        FROM passengers
        WHERE flight_id = 36
    );
-- Who received the call from Bruce. -> Robin
SELECT name
FROM phone_calls,
    people
WHERE caller = "(367) 555-5533"
    AND receiver = phone_number
    AND duration < 60
    AND year = 2021
    AND month = 7
    AND day = 28;
