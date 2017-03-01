package ua.kh.bars.entities;

import javax.persistence.*;
import javax.xml.bind.annotation.XmlType;
import java.util.Date;
import java.util.Objects;

/**
 * Created by green on 3/18/16.
 */

@XmlType(namespace = "http://www.example.org/event")
@Entity
@Table(name = "events")
public class Event {

    @Id
    @GeneratedValue(strategy= GenerationType.IDENTITY)
    private Long id;
    private Date date;
    private String name;
    private int price;
    private String image_url;
    //private EventRating rating;

    @ManyToOne(optional = false, fetch = FetchType.EAGER)
    @JoinColumn(name = "auditorium_id")
    private Auditorium auditorium;

    public Event() {

    }

    public Event(Long id, Date date, String name, int price, String image_url, Auditorium auditorium) {
        this.id = id;
        this.date = date;
        this.name = name;
        this.price = price;
        this.image_url = image_url;
        this.auditorium = auditorium;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getPrice() {
        return price;
    }

    public void setPrice(int price) {
        this.price = price;
    }

    public String getImage_url() {
        return image_url;
    }

    public void setImage_url(String image_url) {
        this.image_url = image_url;
    }

    public Auditorium getAuditorium() {
        return auditorium;
    }

    public void setAuditorium(Auditorium auditorium) {
        this.auditorium = auditorium;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Event event = (Event) o;
        return price == event.price &&
                Objects.equals(id, event.id) &&
                Objects.equals(date, event.date) &&
                Objects.equals(name, event.name) &&
                Objects.equals(auditorium, event.auditorium);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, date, name, price, auditorium);
    }

    @Override
    public String toString() {
        return "Event{" +
                "id=" + id +
                ", date=" + date +
                ", name='" + name + '\'' +
                ", price=" + price +
                ", auditorium=" + auditorium +
                '}';
    }
}
