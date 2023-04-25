# c++ multithreading

mutex, condition_variable 등 공부한 내용 정리

# 데드락을 피하는 방법
* 중첩된 lock 을 피하라
* Lock 을 소유하고 있을 때 유저 코드를 호출하는 것을 피해라 
* Lock 들을 언제나 정해진 순서로 획득해라.

# Reference
* https://modoocode.com/270#page-heading-0

