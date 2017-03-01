package ua.kh.bars.entities;

import javax.persistence.*;
import java.util.Objects;

/**
 * Created by green on 3/18/16.
 */

@Entity
@Table(name = "auditoriums")
public class Auditorium {

    @Id
    @GeneratedValue(strategy= GenerationType.IDENTITY)
    private Long id;
    private String name;
    private int seats;
    private String vipSeats;

    public Auditorium() {

    }

    public Auditorium(Long id, String name, int seats, String vipSeats) {
        this.id = id;
        this.name = name;
        this.seats = seats;
        this.vipSeats = vipSeats;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getSeats() {
        return seats;
    }

    public void setSeats(int seats) {
        this.seats = seats;
    }

    public String getVipSeats() {
        return vipSeats;
    }

    public void setVipSeats(String vipSeats) {
        this.vipSeats = vipSeats;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Auditorium that = (Auditorium) o;
        return seats == that.seats &&
                Objects.equals(id, that.id) &&
                Objects.equals(name, that.name) &&
                Objects.equals(vipSeats, that.vipSeats);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, name, seats, vipSeats);
    }

    @Override
    public String toString() {
        return "Auditorium{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", seats=" + seats +
                ", vipSeats='" + vipSeats + '\'' +
                '}';
    }
}
