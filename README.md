# Elo Rating
Program to rank players/characters based on their performance on bracket-style voting competitions.

The overall position of a character was calculated based on several performance metrics. These metrics include a modified Elo rating, the win rate, the ratio of votes in favour/against and the average number of votes in favour per match.

Elo rating systems are essentially point systems that take into consideration the win/lose probability to decide how many points the players should gain/lose after a match.

The mathematical expression that describes the probability of A beating B is:

    A_win_prob = 1/(1+10^((B_cur_rating - A_cur_rating)/400))

The probability of B beating A is obviously equal to the A losing to B, which means 1 - A\_win\_prob.

If A wins, the mathematical expression that describes the rating update is:

    A_new_rating = A_cur_rating + K * B_win_prob

And if A loses:

    A_new_rating = A_cur_rating - K * A_win_prob

In qualitative terms, K defines how much the rating can vary between matches and this variation depends on the probability of A or B winning the match. You don't lose many points when your chances of winning are low, but you do get a lot of them when you win against the odds.

For players with equal rating, the probability of victory is 50% for both and therefore they’ll either gain or lose K/2 points in their rating after the match. If the probability of A winning is virtually 100% but B wins anyway, B will gain K points.

Elo ratings do not take into account the actual performance of the players within the match. In our case here, there’s a big difference between beating a character by 5000 votes and beating him/her by a couple votes.

The following mathematical expressions depict a modified Elo rating update that also takes the votes into consideration:

If A wins:

    A_new_rating = A_cur_rating + K * B_win_prob * A_votes/(A_votes + B_votes)

If A loses:

    A_new_rating = A_cur_rating - K * A_win_prob * B_votes/(A_votes + B_votes)

With these new expressions, A can only gain/lose K points when there’s an incredible disparity between the ratings AND between the number of votes. Assuming A and B have the same rating, if A only won by a couple votes and the overall share of votes is close to 50%, A will gain K/4 points while B loses K/4 points.

Finding the optimal K value is a rather tricky task. Setting it too low means the ratings change slowly and might not represent the actual performance. Setting it too high makes everything too volatile and recent winners tend to be at the top. I decided to leave it at 100 since it seems to give a decent compromise between the two.

Elo ratings favour players with more matches, since these have had more opportunities to even gain points in the first place. There are some mechanisms that can be employed to alleviate the issue. The first option would be to decrease K as the rating goes up, this is done by some sports federations and online games. The second is to use auxiliary metrics to to give a clearer picture.

In an attempt to make it simultaneously fair for both old and new characters, a geometric mean is done with the following metrics:

Elo Rating: The number that represents how many points the character has accumulated throughout the contests.

Win Rate: The number of victories divided by the number of matches.

Balance: The number of votes in favour divided by the number of votes against.

Attendance: The number of votes in favour divided by the number of matches.

The metrics are all fairly self explanatory. The geometric mean is simply the n-th root of the product of all n metrics. We could perhaps have an argument about dropping the Elo rating entirely (and it wouldn't even change the ranking that much), but since it is the only metric presented here that's based on the specific matchups, I chose to keep it in. Different rating systems could be used. TrueSkill and Glicko are good alternatives, but they're also way more complex.

Using competitions from [Animebracket](https://animebracket.com/), the program was used to rank anime characters:

| Rank | Name                               | Anime                                              | Matches | Wins | Votes In Favour | Votes Against | Win Rate | Attendance | Balance | Elo Rating | Geomean |
|------|------------------------------------|----------------------------------------------------|---------|------|-----------------|---------------|----------|------------|---------|------------|---------|
| 1    | Kaguya Shinomiya                   | Kaguya-sama: Love is War                           | 9       | 9    | 58331           | 28657         | 100%     | 6481       | 2.04    | 1908       | 70.83   |
| 2    | Rem                                | Re:ZERO -Starting Life in Another World-           | 21      | 18   | 162908          | 95315         | 86%      | 7758       | 1.71    | 1963       | 68.72   |
| 3    | Rin Tohsaka                        | Fate/stay night                                    | 38      | 32   | 211466          | 120990        | 84%      | 5565       | 1.75    | 2039       | 63.92   |
| 4    | Megumin                            | KONOSUBA – God’s Blessing on this wonderful world! | 57      | 49   | 252400          | 124534        | 86%      | 4428       | 2.03    | 2143       | 63.77   |
| 5    | Mai Sakurajima                     | Seishun Buta Yarou Series                          | 24      | 22   | 106280          | 58125         | 92%      | 4428       | 1.83    | 2155       | 63.24   |
| 6    | Aqua                               | KONOSUBA – God’s Blessing on this wonderful world! | 53      | 45   | 267298          | 172607        | 85%      | 5043       | 1.55    | 2107       | 61.14   |
| 7    | Holo                               | Spice and Wolf                                     | 93      | 79   | 419441          | 256560        | 85%      | 4510       | 1.63    | 2134       | 60.47   |
| 8    | Hitagi Senjougahara                | Monogatari Series                                  | 44      | 38   | 176288          | 99609         | 86%      | 4007       | 1.77    | 2109       | 59.95   |
| 9    | Shinobu Oshino                     | Monogatari Series                                  | 47      | 40   | 199391          | 120763        | 85%      | 4242       | 1.65    | 2106       | 59.52   |
| 10   | Ryuuko Matoi                       | Kill la Kill                                       | 83      | 70   | 351355          | 206590        | 84%      | 4233       | 1.70    | 2067       | 59.52   |
| 11   | Ochako Uraraka                     | My Hero Academia                                   | 31      | 24   | 158068          | 94100         | 77%      | 5099       | 1.68    | 1835       | 59.06   |
| 12   | Saber                              | Fate/Zero                                          | 79      | 65   | 336109          | 201595        | 82%      | 4255       | 1.67    | 2038       | 58.73   |
| 13   | Lalatina "Darkness" Dustiness Ford | KONOSUBA – God’s Blessing on this wonderful world! | 40      | 32   | 156133          | 84194         | 80%      | 3903       | 1.85    | 1931       | 57.83   |
| 14   | Kurisu Makise                      | Steins;Gate                                        | 34      | 31   | 106885          | 58888         | 91%      | 3144       | 1.82    | 2145       | 57.80   |
| 15   | Yui Yuigahama                      | My youth romantic comedy is wrong as I expected.   | 70      | 57   | 303628          | 192983        | 81%      | 4338       | 1.57    | 2004       | 57.77   |
| 16   | Ai Hayasaka                        | Kaguya-sama: Love is War                           | 17      | 14   | 61841           | 32287         | 82%      | 3638       | 1.92    | 1880       | 57.31   |
| 17   | Emilia                             | Re:ZERO -Starting Life in Another World-           | 43      | 35   | 183216          | 124735        | 81%      | 4261       | 1.47    | 2026       | 56.68   |
| 18   | Mayuri Shiina                      | Steins;Gate                                        | 66      | 53   | 255666          | 160799        | 80%      | 3874       | 1.59    | 1981       | 55.95   |
| 19   | Mikasa Ackerman                    | Attack on Titan                                    | 64      | 51   | 251709          | 159991        | 80%      | 3933       | 1.57    | 1961       | 55.77   |
| 20   | Yoko Littner                       | Gurren Lagann                                      | 59      | 46   | 244833          | 153267        | 78%      | 4150       | 1.60    | 1871       | 55.76   |
| 21   | Violet Evergarden                  | Violet Evergarden                                  | 22      | 18   | 73765           | 41543         | 82%      | 3353       | 1.78    | 1949       | 55.51   |
| 22   | Winry Rockbell                     | Fullmetal Alchemist                                | 67      | 54   | 268419          | 180472        | 81%      | 4006       | 1.49    | 1965       | 55.43   |
| 23   | Momo Yaoyorozu                     | My Hero Academia                                   | 22      | 17   | 109701          | 79984         | 77%      | 4986       | 1.37    | 1762       | 55.24   |
| 24   | Rintarou Okabe                     | Steins;Gate                                        | 69      | 61   | 167901          | 89697         | 88%      | 2433       | 1.87    | 2307       | 55.21   |
| 25   | Asuna Yuuki                        | Sword Art Online                                   | 41      | 31   | 191504          | 141817        | 76%      | 4671       | 1.35    | 1945       | 55.18   |
