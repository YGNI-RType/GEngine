# Prophunt Game

Welcome to **Prophunt**! This game was built with **GEngine**, a custom game engine powered by an **Entity Component System (ECS)** architecture, designed for efficient and modular game development.

## Features

- **Move around** in an immersive 3D environment.
- **Proximity Chat**: Talk to other players near you. (put `voip-start` in the console to start voice chat)
- **Disguise**: Props can transform into different objects to blend in with the environment.
- **Hunt**: Hunters can search for hidden Props and eliminate them.

## Controls

### General Controls

- **W (or Z), A (or Q), S, D:** Move around the environment.
- **Mouse:** Control camera movement to look around.
- **Left Control:** Sprint to move faster.
- **C:** Crouch to hide easier.
- **F5:** Toggle camera mode between first-person, third-person, self look third-person, and free look.
- **1:** Switch to Hunter

### Prop Controls

- **E:** Change to a new object that you are looking at. This allows you to disguise as different items.
- **Left shift:** Lock yourself. This prevents you from moving and rotating, making it easier to blend in with other objects.
- **Space:** Jump to reposition yourself slightly or adjust your view as needed.

### Hunter Controls

- **Left Click:** Shoot to interact with props.

### Additional Commands

- **R:** Reload weapon (if ammunition is limited).
- **Tab:** View player list and current scores.

## How to Play

The game has not built-in rules but here is a general guideline on how to play:

### Game Objective

The goal of Prophunt is simple: **players are split into two teams, Hunters and Props**. Props disguise themselves as various objects in the environment to avoid being detected by Hunters, while Hunters must find and eliminate the hidden Props before time runs out.

### Teams and Roles

- **Props:** You start as a random object in the game environment. Your job is to blend in with other similar objects and avoid detection.
- **Hunters:** Your goal is to find and eliminate all Props before the time limit expires. Use your surroundings and sharp eyes to spot anything that seems out of place.

### Game Flow

1. **Game Start**: At the start of each round, players are assigned to the Prop or Hunter team.
2. **Props Hide**: Props find a hiding spot and disguise as an object in the environment.
3. **Hunters Seek**: Hunters must find and eliminate the Props before the time expires.
4. **Winning Conditions**:
   - **Props win** if at least one Prop remains hidden when time runs out.
   - **Hunters win** if all Props are eliminated before time expires.

## Technical Information

This game is built on **GEngine**, which utilizes an **Entity Component System (ECS)** for performance and flexibility. The ECS design allows for the efficient handling of game objects (Entities) with components like movement, rendering, input, and more. This structure enables fast, scalable gameplay and makes it easy to add new game mechanics.

---

Enjoy hiding, seeking, and the thrill of discovery in Prophunt! Happy hunting and good luck blending in!

## Credits

- **Developers**: Arcod7, Popochounet
- **Assets**: Thomas (pastèque, voiture et chaise)
