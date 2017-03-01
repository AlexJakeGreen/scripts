package ua.kh.bars.service;

import java.util.Collection;
import java.util.Date;
import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.User;
import ua.kh.bars.strategy.DiscountStrategy;

public class DiscountService {

    private Collection<DiscountStrategy> strategies;

    public DiscountService() {
    }

    public DiscountService(Collection<DiscountStrategy> strategies) {
        this.strategies = strategies;
    }

    public int getDiscount(User user, Event event, Date date) {
        int discount = 0;
        for (DiscountStrategy s : strategies) {
            discount += s.getDiscount(user, event, date);
        }
        return discount;
    }

    public void printStrategies() {
        for (DiscountStrategy s : strategies) {
            System.out.println("Strategy: " + s.getName());
        }
    }
}
