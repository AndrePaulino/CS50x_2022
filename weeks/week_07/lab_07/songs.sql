-- Query 01
SELECT name
FROM songs;
-- Query 02
SELECT name
FROM songs
ORDER BY tempo;
-- Query 03
SELECT name
FROM songs
ORDER BY duration_ms DESC
LIMIT 5;
-- Query 04
SELECT name
FROM songs
WHERE energy > 0.75
    AND danceability > 0.75
    AND valence > 0.75;
-- Query 05
SELECT AVG(energy)
FROM songs;
-- Query 06
SELECT songs.name
FROM songs
    JOIN artists ON songs.artist_id = artists.id
WHERE artists.name = 'Post Malone';
-- Query 07
SELECT AVG(energy)
FROM songs
    JOIN artists ON songs.artist_id = artists.id
WHERE artists.name = "Drake";
-- Query 08
SELECT name
FROM songs
WHERE name LIKE "%feat.%";
