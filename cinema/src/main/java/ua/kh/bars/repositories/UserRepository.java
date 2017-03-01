package ua.kh.bars.repositories;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import ua.kh.bars.entities.User;

import java.util.List;

/**
 * Created by green on 3/18/16.
 */
@Repository
public interface UserRepository extends JpaRepository<User, Long> {

    List<User> findByNameContaining(String name);
    User findByEmail(String email);
    User findByUsername(String username);

}
