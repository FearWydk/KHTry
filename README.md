KHTry

# The Resonance \*(working title)\*
===

# 

# \*\*An elemental action RPG built in Unreal Engine 5 with a custom C++ Gameplay Ability System plugin.\*\*

# 

# Solo-developed original project and primary portfolio piece. \*The Resonance\* blends Kingdom Hearts-style contact-gated combos with Final Fantasy XV-style multi-weapon switching — the weapon you wield and the element you channel combine into distinct attack forms.

# 

# > Working title, subject to change. Story, character, and faction names are in active development.

# 

# \---

# 

# \## Premise

# 

# The player is an agent of an organization that publicly hunts "Element Monsters" while secretly manufacturing them. After being ordered to put down someone he loves, he goes rogue — hunting the organization's boss agents, each of whom holds a fragment of a memory he has lost. \*(Narrative is in active rework; this is the current direction.)\*

# 

# \---

# 

# \## Built With

# 

# \- \*\*Unreal Engine 5.8\*\*

# \- \*\*C++\*\* — plugin layer, combat systems, enemy AI

# \- \*\*Blueprint\*\* — game-side logic and UI

# \- \*\*Gameplay Ability System (GAS)\*\*

# 

# \---

# 

# \## Core Systems

# 

# \### GASOline — Custom GAS Plugin

# A from-scratch Gameplay Ability System plugin (`GASOLINE\_API`) providing the reusable combat foundation:

# \- Custom Ability System Component, Attribute Sets, and Character base

# \- \*\*Data-driven ability granting\*\* via a `GASO\_AbilitySet` primary data asset (Lyra-inspired) — abilities, attributes, and effects granted as a single loadout and tracked by handle for clean removal

# \- Shared combat types and stateless rule helpers (`CombatStatics`)

# 

# \### Combat

# \- \*\*Contact-gated combos\*\* — the combo advances only on confirmed hits (KH2-style), driven by a stateful combo manager that broadcasts swing/finisher requests via delegates

# \- \*\*Multi-weapon switching\*\* — Sword / Gun / Lance, FFXV-style

# \- \*\*Weapon + Element = subcategory\*\* — e.g. Sword + Earth = \*Iron\*; each pairing resolves to a distinct attack and finisher form

# \- \*\*Emergent finishers\*\* — the combo history resolves to a finisher type, so mixed-weapon strings can yield mixed finishers

# \- Combat abilities implemented in C++: sword attack, finisher, death, hit-react, enemy attack

# 

# \### Damage Pipeline

# \- A single configurable damage `GameplayEffect` using a \*\*SetByCaller\*\* magnitude — one effect serves every attacker (basic attack, finisher, enemy) with per-ability values, and is future-proofed for affinity-based scaling

# 

# \### Enemy AI

# \- \*\*Behavior Tree + Blackboard\*\* driving patrol / chase / attack, with a \*\*C++ AIController\*\*, custom \*\*BTTasks\*\* (attack, patrol), and a \*\*Decorator\*\* (in-attack-range check)

# \- \*\*AI Perception\*\* (sight) writes the player into the Blackboard target key

# \- Multi-variant attacks (Light / Medium / Heavy) activated through per-variant gameplay-event tags

# 

# \### UI

# \- \*\*Segmented health bar bridged to GAS\*\* — the UI derives everything from current/max health; segments appear as max HP grows, then per-segment capacity scales up past a cap (KH-style)

# \- \*\*KH3-style radial player vitals\*\* built as a UMG material (angle mask + ring mask driven by a dynamic material instance)

# \- \*\*Command menu\*\* (Attack / Magic / Items / Focus) with quick selection and time-dilation-while-choosing (FFVII Rebirth-style)

# 

# \### Inventory

# \- \*\*Fragment-based item system\*\* (Lyra-inspired) — composition over inheritance via a `UItemFragment` base and `ItemDef` data assets that compose behavior (world actor, stackable, usable, equippable) instead of one monolithic item class

# 

# \---

# 

# \## Architecture

# 

# The project separates a \*\*reusable C++ plugin layer\*\* (GASOline — the ability / attribute / combat framework) from the \*\*game layer\*\* (The Resonance-specific abilities, AI, inventory, and Blueprint UI). Patterns are drawn from Epic's Lyra sample where they fit: data-asset-driven ability sets, item fragments, and configuration over hardcoding.

# 

# \---

# 

# \## Status

# 

# Active development toward a \*\*combat vertical slice\*\* — a single boss encounter that demonstrates the full combat loop, a memory-progression hook, and cinematic presentation.

# 

# \- \*\*Implemented:\*\* GAS combat foundation, sword combo + finisher, enemy death / hit-react / AI, segmented health UI, fragment inventory, command menu.

# \- \*\*In progress:\*\* the vertical-slice encounter and the remaining weapon sets.

# 

# \---

# 

# \## About

# 

# Developed by \*\*Josh Brooks\*\* — solo developer targeting a gameplay programmer role.

# Portfolio: \[fearwydk.github.io/portfolio](https://fearwydk.github.io/portfolio)

