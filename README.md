# NeuroRace
NeuroRace is a self learning AI racing simulation built in Unreal Engine using C++ and Blueprints. It uses a genetic algorithm to evolve driving behavior over generations, each cycle spawning 25 cars, evaluates their performance, and breeds the best to replace the worst. Each car is controlled by a neural network that takes in speed, angle difference from the track, and five distance sensors, and outputs turning and throttle values. Fitness is based on how far and how fast each car travels without crashing. What starts as a chaotic collision gradually becomes smooth, high speed driving, entirely learning through iteration.

**Tech stats:**
- Unreal Engine 5
- C++
- Blueprints

### How to play
1. ```Git clone https://github.com/bspafford/NeuroRace```
2. Open the Build folder
3. Run the AIDriving.exe
4. Press B to start the AI Driving

[![Watch Demo](https://bspafford.github.io/AIRacing/media/thumbnail.png)](https://bspafford.github.io/AIRacing/media/AIRacing.mp4)
