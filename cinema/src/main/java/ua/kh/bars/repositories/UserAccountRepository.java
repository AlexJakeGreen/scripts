package ua.kh.bars.repositories;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import ua.kh.bars.entities.User;
import ua.kh.bars.entities.UserAccount;

/**
 * Created by green on 4/3/16.
 */

@Repository
public interface UserAccountRepository extends JpaRepository<UserAccount, Long> {
    UserAccount findByUser(User user);
}
