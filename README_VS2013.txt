SHADOW SPRINT - VS2013 RESULT SCREEN + MOUSE BUTTON VERSION

1. Open ShadowSprint_VS2013.sln in Microsoft Visual Studio 2013.
2. Select Debug / Win32.
3. Build -> Rebuild Solution.
4. Press F5.

RESULT SCREEN FLOW
- Easy death -> Images/easy1.jpg (GAME OVER)
- Easy boss defeated -> Images/easy2.jpg (LEVEL COMPLETE)
- Medium death -> Images/medium1.jpg (GAME OVER)
- Medium boss defeated -> Images/medium2.jpg (LEVEL COMPLETE)

LIVE VALUES
The result screens use the existing game variables:
- score -> FINAL SCORE
- coinCount -> COINS / GOLD COINS COLLECTED

MOUSE BUTTON HIT BOXES (800x400 result images)
Easy Game Over (easy1.jpg):
- RETRY: x 226..394, y 289..343
- QUIT : x 417..585, y 289..343

Easy Level Complete (easy2.jpg):
- RETRY: x 232..399, y 288..343
- RETURN TO CAMP: x 422..590, y 288..343

Medium Game Over (medium1.jpg):
- HOME : x 303..382, y 337..399
- RETRY: x 422..500, y 337..399

Medium Level Complete (medium2.jpg):
- HOME       : x 302..382, y 337..399
- RETRY LEVEL: x 422..501, y 337..399

The result screen appears immediately when finishGame() is called.
The original gameplay, collision, score, coin, life and shooting logic are kept.
