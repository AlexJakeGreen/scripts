package ua.kh.bars.entities;

import javax.persistence.*;
import java.util.Date;
import java.util.Objects;

/**
 * Created by green on 3/18/16.
 */

@Entity
@Table(name = "tickets")
public class Ticket {

    @Id
    @GeneratedValue(strategy= GenerationType.IDENTITY)
    private Long id;

    private int price;
    private Date date;
    private int seat;

    @ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name = "user_id")
    private User user;

    @OneToOne(optional = false, fetch = FetchType.EAGER)
    @JoinColumn(name = "event_id")
    private Event event;

    public Ticket() {

    }

    public Ticket(Long id, int price, Date date, int seat, User user, Event event) {
        this.id = id;
        this.price = price;
        this.date = date;
        this.seat = seat;
        this.user = user;
        this.event = event;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
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

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Ticket ticket = (Ticket) o;
        return price == ticket.price &&
                seat == ticket.seat &&
                Objects.equals(id, ticket.id) &&
                Objects.equals(date, ticket.date) &&
                Objects.equals(user, ticket.user) &&
                Objects.equals(event, ticket.event);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, price, date, seat, user, event);
    }

    @Override
    public String toString() {
        return "Ticket{" +
                "id=" + id +
                ", price=" + price +
                ", date=" + date +
                ", seat=" + seat +
                ", user=" + user +
                ", event=" + event +
                '}';
    }
}
