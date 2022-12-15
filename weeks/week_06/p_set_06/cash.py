from cs50 import get_float

while True:
    change = get_float("How much are owe you? ")
    if change >= 0:
        break

min_coins = 0

if change >= 0.25:
    quarters = change // 0.25
    min_coins += quarters
    change = round(change - (0.25 * quarters), 2)
if change >= 0.10:
    dimes = change // 0.10
    min_coins += dimes
    change = round(change - (0.10 * dimes), 2)
if change >= 0.05:
    nickels = change // 0.05
    min_coins += nickels
    change = round(change - (0.05 * nickels), 2)
if change > 0:
    min_coins += change * 100

print(min_coins)
