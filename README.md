# Near Field Communication for Notebook

## Backstory
A Harvard professor wanted to record patients (users) taking notes on their cancer treatments. This was to ensure they were following up on their treatment.

## Mission
The objective of the device was to covertly log the opening and closing of patient notebook. This data had to include timestamps, be quickly cleared and read from the device, and most importantly, below $20 a notebook.

## Outcome
A system was designed with a STM32L0 microcontroller. The opening and closing of the book was detected with a hall effect sensor and a magnet embedded in the cover.

The STDV04K was used to act as a RFID device that can be read with a cell phone.

![Alt text](./assembled-pic.jpg?raw=true "Title")

## References
To learn more, visit my website

https://hynes-bruell.me/
