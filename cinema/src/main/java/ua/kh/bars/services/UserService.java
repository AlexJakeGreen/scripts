package ua.kh.bars.services;

import ua.kh.bars.entities.User;

import java.util.List;

/**
 * Created by green on 3/18/16.
 */
public interface UserService {

    List<User> findAll();
    User findOne(Long id);
    User save(User user);
    User create(String name, String email, String username, Boolean isEnabled);
    List<User> findUsersByName(String name);
    User findByEmail(String email);
    User findByUsername(String username);
}
