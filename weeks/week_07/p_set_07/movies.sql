-- Query 01
SELECT title
FROM movies
WHERE year = 2008;
-- Query 02
SELECT birth
FROM people
WHERE name = "Emma Stone";
-- Query 03
SELECT title
FROM movies
WHERE year >= 2018
ORDER BY title;
-- Query 04
SELECT COUNT(rating)
FROM ratings
WHERE rating = 10;
-- Query 05
SELECT title,
    year
from movies
WHERE title LIKE "Harry Potter%"
ORDER BY year;
-- Query 06
SELECT AVG(rating)
FROM movies
    JOIN ratings ON movies.id = ratings.movie_id
WHERE movies.year = 2012;
-- Query 07
SELECT movies.title,
    ratings.rating
FROM movies
    JOIN ratings ON movies.id = ratings.movie_id
WHERE movies.year = 2010
ORDER BY ratings.rating DESC,
    movies.title;
-- Query 08
SELECT name
from movies,
    stars,
    people
WHERE title = "Toy Story"
    AND movies.id = stars.movie_id
    AND people.id = stars.person_id;
-- Query 09
SELECT name
FROM movies,
    stars,
    people
WHERE movies.year = 2004
    AND stars.movie_id = movies.id
    AND people.id = stars.person_id
ORDER BY people.birth;
-- Query 10
SELECT DISTINCT name
FROM directors,
    people,
    ratings
WHERE ratings.rating >= 9
    AND ratings.movie_id = directors.movie_id
    AND directors.person_id = people.id;
-- Query 11
SELECT movies.title
FROM movies,
    stars,
    people,
    ratings
WHERE people.name = "Chadwick Boseman"
    AND people.id = stars.person_id
    AND stars.movie_id = movies.id
    AND movies.id = ratings.movie_id
ORDER BY ratings.rating DESC
LIMIT 5;
-- Query 12
SELECT title
FROM movies,
    stars,
    people
WHERE people.name = "Johnny Depp"
    AND movies.id = stars.movie_id
    AND stars.person_id = people.id
    AND title In (
        SELECT title
        FROM movies,
            stars,
            people
        WHERE people.name = "Helena Bonham Carter"
            AND movies.id = stars.movie_id
            AND stars.person_id = people.id
    );
-- Query 13
SELECT DISTINCT name
FROM stars,
    people
WHERE stars.person_id = people.id
    AND movie_id IN (
        SELECT movie_id
        FROM stars,
            people
        WHERE name = "Kevin Bacon"
            AND stars.person_id = people.id
            AND people.birth = 1958
    )
    AND name != "Kevin Bacon";
