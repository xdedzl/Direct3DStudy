//#pragma once
//class Singleton
//{
//private:
//    Singleton()
//    {
//        std::cout << "constructor called!" << std::endl;
//    }
//    Singleton(Singleton&) = delete;
//    Singleton& operator=(const Singleton&) = delete;
//    static Singleton* m_instance_ptr;
//public:
//    ~Singleton()
//    {
//        std::cout << "destructor called!" << std::endl;
//    }
//    static Singleton* get_instance()
//    {
//        if (m_instance_ptr == nullptr)
//        {
//            m_instance_ptr = new Singleton;
//        }
//        return m_instance_ptr;
//    }
//    void use() const { std::cout << "in use" << std::endl; }
//};
//
//Singleton* Singleton::m_instance_ptr = nullptr;