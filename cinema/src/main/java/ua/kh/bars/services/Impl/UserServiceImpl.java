package ua.kh.bars.services.Impl;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import ua.kh.bars.entities.User;
import ua.kh.bars.repositories.UserRepository;
import ua.kh.bars.services.UserService;

import java.util.Date;
import java.util.List;

/**
 * Created by green on 3/18/16.
 */

@Service
@Transactional
public class UserServiceImpl implements UserService {

    @Autowired
    private UserRepository userRepository;

    public List<User> findAll() {
        return userRepository.findAll();
    }

    public User findOne(Long id) {
        return userRepository.findOne(id);
    }

    public User save(User user) {
        User savedUser = userRepository.saveAndFlush(user);
        return savedUser;
    }

    public User create(String name, String email, String username, Boolean isEnabled) {
        User user = new User();
        user.setName(name);
        user.setEmail(email);
        user.setBirthday(new Date());
        user.setRoles("ROLE_USER");
        user.setUsername(username);
        user.setEnabled(isEnabled);
        user.setPassword("7110eda4d09e062aa5e4a390b0a572ac0d2c0220");
        userRepository.save(user);
        return user;
    }

    // (Made it simple due to lack of time)
    public List<User> findUsersByName(String name) {
        return userRepository.findByNameContaining(name);
    }

    public User findByEmail(String email) {
        return userRepository.findByEmail(email);
    }

    public User findByUsername(String username) {
        return userRepository.findByUsername(username);
    }

}
