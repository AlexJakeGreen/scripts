package ua.kh.bars.service;

import ua.kh.bars.dao.UserDAO;
import ua.kh.bars.entity.User;
import java.util.Date;

public class UserService {

    private UserDAO userDAO;

    public UserService(UserDAO userDAO) {
        this.userDAO = userDAO;
    }

    public Iterable<User> findAll() {
        return userDAO.findAll();
    }

    public User register(String email, String name, Date birthday) {
        User user = new User(email, name, birthday);
        userDAO.save(user);
        return user;
    };

    public void remove(User user) {
        userDAO.delete(user);
    };

    public User getById(Long id) {
        return userDAO.findOne(id);
    };

    public User getUserByEmail(String email) {
        return userDAO.findByEmail(email);
    };

    public Iterable<User> getUsersByName(String name) {
        return userDAO.findByName(name);
    };

    public int getBookedTickets(User user) {
        return user.getTickets();
    };

    public Long getCount() {
        return userDAO.count();
    }
}
