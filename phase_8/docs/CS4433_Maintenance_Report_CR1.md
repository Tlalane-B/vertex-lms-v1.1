# CS4433 Software Engineering: Maintenance Report (Change Request CR1)

**Project Title:** Vertex LMS (Offline-Capable Learning Management System)  
**Version:** v1.1 (Maintenance Release)  
**Author:** Kekeletso Eric Makhutla  
**Institution:** National University of Lesotho (NUL), Department of Computer Science  

---

## 1. Change Request Description (CR1)
> *"A lecturer should be able to extend the deadline for a whole class, not just an individual student, and extensions should be shown on the student's assignment list."*

## 2. Classification & Impact Analysis
- **Classification:** Perfective Maintenance.
- **Impacted Components:** `AssignmentRepository`, `AssignmentService`, `ConsoleUI`, and `assignments` table schema.

## 3. Regression Testing Results
All regression tests validating class-wide deadline extensions and student view synchronization have passed successfully.
