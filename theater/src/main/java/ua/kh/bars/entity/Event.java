package ua.kh.bars.entity;

import java.util.Date;
import ua.kh.bars.entity.Auditorium;

public class Event {
    private Long id;
    private Date date;
    private String name;
    private int price;
    private EventRating rating;
    private Auditorium auditorium;

    public Event() {
    }
    
    public Event(Date date, String name, int price, EventRating rating) {
        this.date = date;
        this.name = name;
        this.price = price;
        this.rating = rating;
        this.auditorium = null;
        this.id = null;
    }

    public Event(Date date, String name, int price, EventRating rating, Auditorium auditorium) {
        this(date, name, price, rating);
        this.auditorium = auditorium;
    }

    public Event(Long id, Date date, String name, int price, EventRating rating, Auditorium auditorium) {
        this(date, name, price, rating, auditorium);
        this.id = id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Long getId() {
        return id;
    }
   
    public void setName(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setPrice(int price) {
        this.price = price;
    }

    public int getPrice() {
        return this.price;
    }

    public void setRating(EventRating rating) {
        this.rating = rating;
    }

    public EventRating getRating() {
        return rating;
    }

    public void setAuditorium(Auditorium auditorium) {
        this.auditorium = auditorium;
    }

    public Auditorium getAuditorium() {
        return auditorium;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public Date getDate() {
        return date;
    }

    public String toString() {
        return "Event: {id: '" + id
            + "', date: '" + date
            + "', name: '"+ name
            + "', price: '" + price
            + "', rating: '" + rating
            + "', auditorium: '" + ( auditorium != null ? auditorium.getName() : "__NOT_ASSIGNED__" ) + "'}";
    }
}
