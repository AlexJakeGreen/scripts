package ua.kh.bars.service;

import ua.kh.bars.dao.AuditoriumDAO;
import ua.kh.bars.entity.Auditorium;

public class AuditoriumService {

    AuditoriumDAO auditoriumDAO;

    public AuditoriumService(AuditoriumDAO auditoriumDAO) {
        this.auditoriumDAO = auditoriumDAO;
    }

    public Iterable<Auditorium> getAuditoriums() {
        return auditoriumDAO.findAll();
    }

    public int getSeatsNumber(Auditorium auditorium) {
        return auditorium.getSeats();
    }

    public String[] getVipSeats(Auditorium auditorium) {
        return auditorium.getVipSeats();
    }

}
