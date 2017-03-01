package ua.kh.bars.strategy;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.User;

class BirthdayStrategy implements DiscountStrategy {
    private String name;
    private int defaultDiscount;
    private int birthdayDiscount;

    public BirthdayStrategy(int defaultDiscount, int specialDiscount) {
        this.defaultDiscount = defaultDiscount;
        this.birthdayDiscount = specialDiscount;
        this.name = "Birthday - " + specialDiscount + "%";
    }

    public int getDiscount(User user, Event event, Date date) {

        Calendar cal = Calendar.getInstance();
        cal.setTime(user.getBirthday());
        int userMonth = cal.get(Calendar.MONTH);
        int userDay = cal.get(Calendar.DAY_OF_MONTH);

        cal.setTime(date);
        int eventMonth = cal.get(Calendar.MONTH);
        int eventDay = cal.get(Calendar.DAY_OF_MONTH);

        if (userMonth == eventMonth && userDay == eventDay) {
            return birthdayDiscount;
        }
        return defaultDiscount;
    }

    public String getName() {
        return name;
    }
}
