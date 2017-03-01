package ua.kh.bars.strategy;

import java.util.Date;
import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.User;

public class Every10TicketStrategy implements DiscountStrategy {
    private String name;
    private int defaultDiscount;
    private int specialDiscount;

    public Every10TicketStrategy(int defaultDiscount, int specialDiscount) {
        this.defaultDiscount = defaultDiscount;
        this.specialDiscount = specialDiscount;
        this.name = "Each 10th ticket - " + specialDiscount + "% free!";
    }

    public int getDiscount(User user, Event event, Date date) {
        if ( (user.getTickets() + 1) % 10 == 0) {
            return specialDiscount;
        }
        return defaultDiscount;
    }

    public String getName() {
        return name;
    }
}
