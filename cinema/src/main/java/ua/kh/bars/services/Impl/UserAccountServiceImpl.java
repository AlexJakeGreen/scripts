package ua.kh.bars.services.Impl;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import ua.kh.bars.entities.User;
import ua.kh.bars.entities.UserAccount;
import ua.kh.bars.repositories.UserAccountRepository;
import ua.kh.bars.services.UserAccountService;

/**
 * Created by green on 4/3/16.
 */

@Service
public class UserAccountServiceImpl implements UserAccountService {

    @Autowired
    UserAccountRepository userAccountRepository;

    public int getPrepaid(User user) {
        UserAccount userAccount = userAccountRepository.findByUser(user);
        return userAccount.getMoneyPrepaid();
    }

    public boolean withdraw (User user, int money) throws RuntimeException {
        UserAccount userAccount = userAccountRepository.findByUser(user);
        int newMoney = userAccount.getMoneyPrepaid() - money;
        if (newMoney < 0) {
            return false;
        };
        userAccount.setMoneyPrepaid(newMoney);
        userAccountRepository.save(userAccount);
        return true;
    }

}
