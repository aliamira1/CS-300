# CS-300

# What was the problem you were solving in the projects for this course?
I developed a course management system to efficiently store and retrieve course information including prerequisites. 

# How did you approach the problem? Consider why data structures are important to understand.
I carefully evaluated three data structure options before implementation: hash table, vector, and binary search tree. A hash table was chosen because it offers O(1) average-case time complexity for lookups and insertions, compared to O(n) lookup time for vectors and O(log n) for binary search trees. Understanding these data structures was crucial because while vectors would be simpler to implement and binary search trees would maintain sorted order, the hash table's constant-time operations made it optimal for frequent course lookups and insertions in a course management system. This analysis demonstrates how choosing the right data structure significantly impacts program performance.

# How did you overcome any roadblocks you encountered while going through the activities or project?
The main challenges included handling memory management properly (solved by implementing proper copy constructor and destructor), dealing with input buffer issues (resolved using proper buffer clearing techniques), and ensuring data validation (addressed by adding robust error checking). Breaking down each problem into smaller components helped tackle these issues systematically.

# How has your work on this project expanded your approach to designing software and developing programs?
This project deepened my understanding of data structures, particularly the hash table, and its importance for efficient data access and retrieval. Working with hash tables taught me about collision management, hashing functions, and the importance of balancing simplicity with performance. Additionally, the project emphasized the value of modular design and encapsulation, which improves both the readability and scalability of software.

# How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?
This project emphasized the importance of designing with modularity, making the code more organized and easier to understand. By implementing custom data structures like the hash table, I learned to focus on clear organization, using encapsulation to keep related functionality within specific classes. The experience also reinforced the need for readability through comments and consistent naming, making the program adaptable for future enhancements or modifications.
