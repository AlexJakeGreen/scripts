package ua.kh.bars.entity;

import java.util.Collection;

public class Auditorium {
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

    public void setSeats(int seats) {
        this.seats = seats;
    }

    public int getSeats() {
        return seats;
    }

    public void setVipSeats(String vipSeats) {
        this.vipSeats = vipSeats;
    }

    public String[] getVipSeats() {
        if(vipSeats != null) {
            return vipSeats.split(",");
        };
        return new String[]{};
    }

    public String toString() {
        return "Auditorium: {name: '" + name
            + "', seats: '" + seats
            + "', vipSeats: '" + vipSeats + "'}";
    }
}
