package ua.kh.bars.services;

import ua.kh.bars.entities.User;

/**
 * Created by green on 4/3/16.
 */
public interface UserAccountService {

    int getPrepaid(User user);
    boolean withdraw(User user, int money);

}
