package ua.kh.bars.entity;

import java.util.Date;
import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.User;

public class Ticket {
    private Long id;
    private User user;
    private Event event;
    private int price;
    private Date date;
    private int seat;

    public Ticket() {
    }

    public Ticket(User user, Event event, int price, int seat) {
        this.user = user;
        this.event = event;
        this.price = price;
        this.seat = seat;
        this.date = null;
    }

    public Ticket(User user, Event event, int price, int seat, Date date) {
        this(user, event, price, seat);
        this.date = date;
    }

    public Ticket(Long id, User user, Event event, int price, int seat, Date date) {
        this(user, event, price, seat, date);
        this.id = id;
    }
    
    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public Event getEvent() {
        return event;
    }

    public void setEvent(Event event) {
        this.event = event;
    }

    public int getPrice() {
        return price;
    }

    public void setPrice(int price) {
        this.price = price;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public int getSeat() {
        return seat;
    }

    public void setSeat(int seat) {
        this.seat = seat;
    }

    public String toString() {
        return "Ticket: {user: '" + user.getName()
            + "', event: '" + event.getName()
            + "', price: '" + price
            + "', seat: '" + seat
            + "', date: '" + date +"'}";
    }
}
