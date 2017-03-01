package ua.kh.bars.strategy;

import java.util.Date;
import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.User;

public interface DiscountStrategy {

    public int getDiscount(User user, Event event, Date date);

    public String getName();

}
